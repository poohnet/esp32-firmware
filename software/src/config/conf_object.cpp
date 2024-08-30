/* esp32-firmware
 * Copyright (C) 2020-2024 Erik Fleckstein <erik@tinkerforge.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config/private.h"

bool Config::ConfObject::slotEmpty(size_t i)
{
    return object_buf[i].schema == nullptr;
}

Config::ConfObject::Slot *Config::ConfObject::allocSlotBuf(size_t elements)
{
    return new Config::ConfObject::Slot[elements];
}

void Config::ConfObject::freeSlotBuf(Config::ConfObject::Slot *buf)
{
    delete[] buf;
}

Config *Config::ConfObject::get(const String &needle)
{
    const auto *slot = this->getSlot();
    const auto schema = slot->schema;
    const auto size = schema->length;

    const auto needle_length = needle.length();

    for (size_t i = 0; i < size; ++i) {
        if (schema->keys[i].length != needle_length)
            continue;

        if (memcmp(schema->keys[i].val, needle.c_str(), needle_length) == 0)
            return &slot->values[i];
    }

    char *message;
    esp_system_abort(asprintf(&message, "Config key %s not found!", needle.c_str()) < 0 ? "" : message);
}

const Config *Config::ConfObject::get(const String &needle) const
{
    const auto *slot = this->getSlot();
    const auto schema = slot->schema;
    const auto size = schema->length;

    const auto needle_length = needle.length();

    for (size_t i = 0; i < size; ++i) {
        if (schema->keys[i].length != needle_length)
            continue;

        if (memcmp(schema->keys[i].val, needle.c_str(), needle_length) == 0)
            return &slot->values[i];
    }

    char *message;
    esp_system_abort(asprintf(&message, "Config key %s not found!", needle.c_str()) < 0 ? "" : message);
}

const Config::ConfObject::Slot *Config::ConfObject::getSlot() const { return &object_buf[idx]; }
Config::ConfObject::Slot *Config::ConfObject::getSlot() { return &object_buf[idx]; }

Config::ConfObject::ConfObject(std::vector<std::pair<String, Config>> &&val)
{
    auto len = val.size();

    auto schema = (ConfObjectSchema *) heap_caps_malloc(sizeof(ConfObjectSchema) + len * sizeof(ConfObjectSchema::Key), MALLOC_CAP_32BIT);
    schema->length = len;

    size_t buf_len = 0;
    for (int i = 0; i < len; ++i) {
        schema->keys[i].length = val[i].first.length();
        buf_len += schema->keys[i].length + 1;
    }

    char *key_buf = (char *)heap_caps_calloc_prefer(buf_len, sizeof(char), 2, MALLOC_CAP_SPIRAM, MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL);

    size_t written = 0;
    for (int i = 0; i < len; ++i) {
        schema->keys[i].val = key_buf + written;
        memcpy(key_buf + written, val[i].first.c_str(), schema->keys[i].length);
        written += schema->keys[i].length;
        key_buf[written] = '\0';
        ++written;
    }

    idx = nextSlot<Config::ConfObject>(object_buf, object_buf_size);
    auto *slot = this->getSlot();
    slot->schema = schema;
    slot->values = heap_alloc_array<Config>(len);

    for (int i = 0; i < len; ++i) {
        this->getSlot()->values[i] = std::move(val[i].second); //TODO: move here?
    }
}

Config::ConfObject::ConfObject(const ConfObject &cpy)
{
    idx = nextSlot<Config::ConfObject>(object_buf, object_buf_size);

    // TODO: could we just use *this = cpy here?

    // We have to mark this slot as in use here:
    // (by setting the schema pointer first)
    // This object could contain a nested object that will be copied over
    // The inner object's copy constructor then takes the first free slot, i.e.
    // ours if we don't mark it as in use first.
    this->getSlot()->schema = cpy.getSlot()->schema;

    const auto len = cpy.getSlot()->schema->length;
    auto values = heap_alloc_array<Config>(len);
    for (int i = 0; i < len; ++i)
        values[i] = cpy.getSlot()->values[i];

    // Must call getSlot() again because any reference would be invalidated
    // if the copy triggers a move of the slots.
    this->getSlot()->values = std::move(values);
}

Config::ConfObject::~ConfObject()
{
    if (idx == std::numeric_limits<decltype(idx)>::max())
        return;

    auto *slot = this->getSlot();
    slot->schema = nullptr;
    slot->values = nullptr;
}

Config::ConfObject &Config::ConfObject::operator=(const ConfObject &cpy)
{
    if (this == &cpy)
        return *this;

    // We have to mark this slot as in use here:
    // (by setting the schema pointer first)
    // This object could contain a nested object that will be copied over
    // The inner object's copy constructor then takes the first free slot, i.e.
    // ours if we don't mark it as in use first.
    this->getSlot()->schema = cpy.getSlot()->schema;

    const auto len = cpy.getSlot()->schema->length;
    auto values = heap_alloc_array<Config>(len);
    for (int i = 0; i < len; ++i)
        values[i] = cpy.getSlot()->values[i];

    // Must call getSlot() again because any reference would be invalidated
    // if the copy triggers a move of the slots.
    this->getSlot()->values = std::move(values);

    return *this;
}

Config::ConfObject::ConfObject(ConfObject &&cpy) {
    this->idx = cpy.idx;
    cpy.idx = std::numeric_limits<decltype(idx)>::max();
}

Config::ConfObject &Config::ConfObject::operator=(ConfObject &&cpy) {
    this->idx = cpy.idx;
    cpy.idx = std::numeric_limits<decltype(idx)>::max();
    return *this;
}
