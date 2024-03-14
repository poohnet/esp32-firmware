import os
import sys
import time
from threading import Thread

from pathlib import Path

from provision_common.provision_common import *

from tinkerforge.ip_connection import IPConnection, base58encode, base58decode, BASE58
from tinkerforge.bricklet_industrial_quad_relay_v2 import BrickletIndustrialQuadRelayV2
from tinkerforge.bricklet_rgb_led_v2 import BrickletRGBLEDV2
from tinkerforge.bricklet_temperature_v2 import BrickletTemperatureV2

SERIAL_SETTLE_DELAY = 1

class ThreadWithReturnValue(Thread):
    def __init__(self, group=None, target=None, name=None,
                args=(), kwargs={}, Verbose=None):
        Thread.__init__(self, group, target, name, args, kwargs)
        self._return = None
        self._success = False
        self._exception = None

    def run(self):
        if self._target is not None:
            try:
                self._return = self._target(*self._args, **self._kwargs)
                self._success = True
            except BaseException as e:
                self._exception = e

    def join(self, *args):
        Thread.join(self, *args)
        return self._success, self._return, self._exception

def test_bricklet_ports_warp3(ipcon):
    enums = enumerate_devices(ipcon)

    expected = {
        'A': BrickletTemperatureV2.DEVICE_IDENTIFIER,
        'B': BrickletTemperatureV2.DEVICE_IDENTIFIER,
        'C': BrickletRGBLEDV2.DEVICE_IDENTIFIER,
        'D': BrickletTemperatureV2.DEVICE_IDENTIFIER,
    }

    if len(enums) != 4:
        fatal_error("Expected 4 bricklets but found {}".format("\n\t".join("Port {}: {}".format(x.position, x.device_identifier) for x in enums)))

    for x in enums:
        if expected[x.position] != x.device_identifier:
            fatal_error("Expected {} at port {} but found {}".format(expected[x.position], x.position, "\n\t".join("Port {}: {}".format(x.position, x.device_identifier) for x in enums)))
        expected.pop(x.position)

    # This should never happen, as we have 4 expected bricklets and 4 enumerations
    if len(expected) > 0:
        fatal_error("Expected other bricklets but found {}".format("\n\t".join("Port {}: {}".format(x.position, x.device_identifier) for x in enums)))

    enums = sorted(enums, key=lambda x: x.position)

    bricklets = {enum.position: BrickletTemperatureV2(enum.uid, ipcon) if enum.device_identifier == BrickletTemperatureV2.DEVICE_IDENTIFIER else BrickletRGBLEDV2(enum.uid, ipcon) for enum in enums}
    error_count = 0

    temps = []

    for port, bricklet in bricklets.items():
        bricklet.set_response_expected_all(True)

        try:
            if port == 'C':
                bricklet.set_rgb_value(63, 63, 0)
                time.sleep(0.5)
                if bricklet.get_rgb_value() == (63, 63, 0):
                    bricklet.set_rgb_value(0, 0, 0)
                else:
                    print(red("Setting color failed on port {}.".format(port)))
                    error_count += 1
            else:
                temps.append(bricklet.get_temperature() / 100)
        except Exception as e:
            print("Failed to communicate with bricklet on port {}: {}".format(port, e))
            error_count += 1

    if error_count != 0:
        fatal_error("")

    return sum(temps) / len(temps), bricklets['C'].uid_string

def connect_ethernet(ip):
    print(f"Connecting via ethernet to {ip}", end="")
    for i in range(30):
        start = time.time()
        try:
            with urllib.request.urlopen(f"http://{ip}/info/modules", timeout=1) as f:
                f.read()
                break
        except:
            pass
        t = max(0, 1 - (time.time() - start))
        time.sleep(t)
        print(".", end="")
    else:
        fatal_error("Failed to connect via ethernet!")

    print(" Connected.")

def test_rtc_time(ip):
    print("Testing RTC")
    try:
        with urllib.request.urlopen(f"http://{ip}/rtc/time", timeout=10) as f:
            t = json.loads(f.read())
            esp_time = datetime.datetime(t["year"], t["month"], t["day"], t["hour"], t["minute"], t["second"], tzinfo=datetime.timezone.utc)
    except Exception as e:
        print(e)
        fatal_error("Failed to read RTC time!")

    print(esp_time, datetime.datetime.now(datetime.timezone.utc))

    if (esp_time - datetime.datetime.now(datetime.timezone.utc)).total_seconds() > 3:
        fatal_error("RTC time wrong!")

def get_esp_ssid(serial_port, result):

    print("Checking ESP state")
    mac_address = check_if_esp_is_sane_and_get_mac(allowed_revision=3.1, override_port=serial_port)
    print("MAC Address is {}".format(mac_address))
    result["mac"] = mac_address

    set_voltage_fuses, set_block_3, passphrase, uid = get_espefuse_tasks(override_port=serial_port)
    if set_voltage_fuses or set_block_3:
        fatal_error("Fuses are not set. Re-run stage 0!")

    esptool(["--after", "hard_reset", "run"], override_port=serial_port)

    result["uid"] = uid

    ssid = "warp3-" + uid

    return ssid, passphrase

def test_wifi(ssid, passphrase, ethernet_ip, result):
    print("Waiting for ESP wifi. Takes about one minute.")
    if not wait_for_wifi(ssid, 90):
        fatal_error("ESP wifi not found after 90 seconds")

    print("Testing ESP Wifi.")
    with wifi(ssid, passphrase):
        req = urllib.request.Request("http://10.0.0.1/ethernet/config_update",
                                     data=json.dumps({"enable_ethernet":True,
                                                      "ip": ethernet_ip,
                                                      "gateway":"192.168.1.1",
                                                      "subnet":"255.255.0.0",
                                                      "dns":"192.168.1.1",
                                                      "dns2":"0.0.0.0"}).encode("utf-8"),
                                     method='PUT',
                                     headers={"Content-Type": "application/json"})
        try:
            with urllib.request.urlopen(req, timeout=10) as f:
                f.read()
        except Exception as e:
            print(e)
            fatal_error("Failed to set ethernet config!")
        req = urllib.request.Request("http://10.0.0.1/reboot", data=b'null', method='PUT', headers={"Content-Type": "application/json"})
        try:
            with urllib.request.urlopen(req, timeout=10) as f:
                f.read()
        except Exception as e:
            print("Failed to initiate reboot! Attempting to connect via ethernet anyway.")

        result["wifi_test_successful"] = True

def run_stage_1_tests(serial_port, ethernet_ip, power_off_fn, power_on_fn, result):
    connect_ethernet(ethernet_ip)

    req = urllib.request.Request(f"http://{ethernet_ip}/info/version")
    try:
        with urllib.request.urlopen(req, timeout=10) as f:
            fw_version = json.loads(f.read().decode("utf-8"))["firmware"].split("-")[0]
    except Exception as e:
        fatal_error("Failed to read firmware version!")

    try:
        with urllib.request.urlopen(f"http://{ethernet_ip}/hidden_proxy/enable", timeout=10) as f:
            f.read()
    except Exception as e:
        print(e)
        fatal_error("Failed to enable hidden_proxy!")

    time.sleep(3)
    ipcon = IPConnection()
    ipcon.connect(ethernet_ip, 4223)
    result["ethernet_test_successful"] = True
    print("Connected. Testing bricklet ports")

    avg_bricklet_temp, rgb_led_uid = test_bricklet_ports_warp3(ipcon)
    result["bricklet_port_test_successful"] = True

    ipcon.disconnect()

    print("Testing temperature sensor")
    try:
        with urllib.request.urlopen(f"http://{ethernet_ip}/esp32/temperature", timeout=10) as f:
            esp_temp = float(json.loads(f.read())["temperature"]) / 100
    except Exception as e:
        print(e)
        fatal_error("Failed to read temperature value!")

    if abs(avg_bricklet_temp - esp_temp) > 15:
        fatal_error(f"ESP temperature sensor value not in expected range: {esp_temp=} {avg_bricklet_temp=}")

    result["temperature_test_successful"] = True

    test_rtc_time(ethernet_ip)

    print("Testing RTC supercap")

    req = urllib.request.Request(f"http://{ethernet_ip}/ntp/config_update",
                                data=json.dumps({"enable":False,
                                                "use_dhcp":None,
                                                "timezone":None,
                                                "server":None,
                                                "server2":None}).encode("utf-8"),
                                method='PUT',
                                headers={"Content-Type": "application/json"})
    try:
        with urllib.request.urlopen(req, timeout=1) as f:
            f.read()
    except:
        pass

    power_off_fn()
    time.sleep(10)
    power_on_fn()

    connect_ethernet(ethernet_ip)

    test_rtc_time(ethernet_ip)

    result["rtc_test_successful"] = True

    try:
        with urllib.request.urlopen(f"http://{ethernet_ip}/hidden_proxy/enable", timeout=10) as f:
            f.read()
    except Exception as e:
        print(e)
        fatal_error("Failed to enable hidden_proxy!")

    time.sleep(3)

    ipcon.connect(ethernet_ip, 4223)

    result["tests_successful"] = True
    result["end"] = now()

    rgb_led = BrickletRGBLEDV2(rgb_led_uid, ipcon)
    rgb_led.set_rgb_value(0, 127, 0)

    return ipcon, rgb_led_uid


def print_label(ssid, passphrase, stage_1_test_report):
    with open("{}_{}_report_stage_1.json".format(ssid, now().replace(":", "-")), "w") as f:
        json.dump(stage_1_test_report, f, indent=4)

    label_success = "n"
    while label_success != "y":
        run(["python3", "print-esp32-label.py", ssid, passphrase, "-c", "3"])
        label_prompt = "Stick one label on the ESP, put ESP{} in the ESD bag. Press n to retry printing the label{}. [y/n]".format(
                " and the other two labels",
                "s")

        label_success = input(label_prompt)
        while label_success not in ("y", "n"):
            label_success = input(label_prompt)

    print('Done!')

def reset_ntp_config_fn(ethernet_ip):
    def inner(ethernet_ip):
        try:
            with urllib.request.urlopen(f"http://{ethernet_ip}/ntp/config_reset", timeout=1) as f:
                f.read()
        except:
            fatal_error("Failed to re-enable NTP")
    return lambda: inner(ethernet_ip)

def reset_ethernet_config_fn(ethernet_ip):
    def inner(ethernet_ip):
        try:
            with urllib.request.urlopen(f"http://{ethernet_ip}/ethernet/config_reset", timeout=1) as f:
                f.read()
        except:
            fatal_error("Failed to re-enable NTP")
    return lambda: inner(ethernet_ip)

def main():
    cleanup = []

    ipcon = IPConnection()
    ipcon.connect("localhost", 4223)
    devices = enumerate_devices(ipcon)

    iqr = None

    for d in devices:
        if d.device_identifier == BrickletIndustrialQuadRelayV2.DEVICE_IDENTIFIER:
            iqr = BrickletIndustrialQuadRelayV2(d.uid, ipcon)
            break
    else:
        print("No industrial quad relay 2.0 bricklet found!")
        sys.exit(-1)

    iqr.set_value([True, True, True, True])
    time.sleep(SERIAL_SETTLE_DELAY)

    relay_to_serial = {}

    all_serials = set(Path('/dev/').glob('ttyUSB*'))

    for i in range(4):
        iqr.set_selected_value(i, False)
        time.sleep(SERIAL_SETTLE_DELAY)
        new_all_serials = set(Path('/dev/').glob('ttyUSB*'))
        missing = all_serials - new_all_serials
        if len(missing) == 1:
            relay_to_serial[i] = list(missing)[0]
        elif len(missing) == 0:
            print(f"Quad relay pin {i} not plugged in")
        else:
            print(f"Switching quad relay pin {i} off removed more than one serial console: {missing} Is the hardware plugged in correctly?")

        all_serials = new_all_serials

    print(relay_to_serial)

    iqr.set_value([True, True, True, True])
    time.sleep(SERIAL_SETTLE_DELAY)

    #fixme does capturing v work here?
    threads = []
    for k, v in relay_to_serial.items():
        t = ThreadWithReturnValue(target=lambda: \
            subprocess.run(
                ['python', 'provision_stage_0_warp2.py', ' ../../firmwares/bricks/warp3_charger/brick_warp3_charger_firmware_latest.bin', v, "warp3"],
                capture_output=True,
                encoding='utf-8'))
        t.start()
        threads.append((k, v, t))

    for k, v, t in threads:
        success, result, exception = t.join()
        if not success:
            print(red(f"Failed to run stage 0 for {k} {v}: {exception}"))
            relay_to_serial.pop(k)

        if result.returncode != 0:
            print(red(f"Failed to run stage 0 for {k} {v}: {result.stdout}\n{result.stderr}"))
            relay_to_serial.pop(k)

    threads.clear()

    def get_esp_ssid_fn(serial_port, result):
        return lambda: get_esp_ssid(serial_port, result)

    test_reports = {k: {"start": now()} for k in relay_to_serial.keys()}

    relay_to_ssid = {}
    relay_to_passphrase = {}

    for k, v in relay_to_serial.items():
        t = ThreadWithReturnValue(target=get_esp_ssid_fn(v, test_reports[k]))
        t.start()
        threads.append((k, v, t))

    for k, v, t in threads:
        success, result, exception = t.join()
        if not success:
            print(red(f"Failed to run get_esp_ssid for {k} {v}: {exception}"))
            relay_to_serial.pop(k)
        else:
            relay_to_ssid[k] = result[0]
            relay_to_passphrase[k] = result[1]

    threads.clear()

    run(["systemctl", "restart", "NetworkManager.service"])
    run(["sudo" ,"iw", "reg", "set", "DE"])

    for k, v in list(relay_to_serial.items()):
        try:
            cleanup.append(reset_ethernet_config_fn(f"192.168.1.{9 + k}"))
            test_wifi(relay_to_ssid[k], relay_to_passphrase[k], f"192.168.1.{9 + k}", test_reports[k])
        except BaseException as e:
            print(red(f"Failed to test WiFi for {k} {v}: {e}"))
            relay_to_serial.pop(k)

    def run_stage_1_tests_fn(serial_port, ethernet_ip, relay_pin, test_report):
        return lambda: run_stage_1_tests(serial_port, ethernet_ip, lambda: iqr.set_selected_value(relay_pin, False), lambda: iqr.set_selected_value(relay_pin, True), test_report)

    for k, v in relay_to_serial.items():
        cleanup.append(reset_ntp_config_fn(f"192.168.1.{9 + k}"))
        t = ThreadWithReturnValue(target=run_stage_1_tests_fn(v, f"192.168.1.{9 + k}", k, test_reports[k]))
        t.start()
        threads.append((k, v, t))

    relay_to_rgb_led = {}

    for k, v, t in threads:
        success, result, exception = t.join()
        if not success:
            print(red(f"Failed to run run_stage_1_tests for {k} {v}: {exception}"))
            relay_to_serial.pop(k)
        else:
            ipcon, rgb_led_uid = result
            rgb_led = BrickletRGBLEDV2(rgb_led_uid, ipcon)

            relay_to_rgb_led[k] = rgb_led

    print("Running cleanup functions")
    for fn in cleanup:
        fn()

    for k, v in list(relay_to_rgb_led.items()):
            stop_event = threading.Event()
            blink_thread = threading.Thread(target=blink_thread_fn, args=([v], stop_event))
            blink_thread.start()

            print_label(relay_to_ssid[k], relay_to_passphrase[k], test_reports[k])

            stop_event.set()
            blink_thread.join()

    threads.clear()

if __name__ == "__main__":
    main()
