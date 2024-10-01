#!/usr/bin/env python3

# See https://github.com/pypa/pip/issues/10793#issuecomment-1017029048
import argparse
from pathlib import Path
import sys
from packaging.utils import parse_wheel_filename
import platform


def diagnose_unsupported(p: Path) -> str:
    p = Path(p)
    if not p.exists():
        return f"File '{p}' does not exist"
    if not p.is_file():
        return f"'{p}' is not a file"
    if p.suffix != ".whl":
        return f"'{p}' has incorrect suffix; the suffix must be .whl"

    # The wheel filename must parse:
    _, _, _, tags = parse_wheel_filename(p.name)

    tag = None
    for t in tags:
        tag = t

    # Is a debug wheel being loaded in a non-debug interpreter?
    if tag.abi.endswith("d"):
        if not sys.flags.debug:
            return f"The ABI of the wheel is {tag.abi}, which is a debug wheel. However, the python interpeter does not have the debug flags set."
    # Is a cpython wheel being loaded by a non-cpython interpreter?
    if tag.abi.startswith("cp"):
        if sys.implementation.name != "cpython":
            return f"The ABI of the wheel '{p}' requires cpython, but the system implementation is {sys.implementation.name}"

    # If the interpreter is version intolerant, what interpreter should it be using?
    idx = tag.interpreter.find("3")
    if idx >= 0 and idx < len(tag.interpreter) - 1:
        supported_minor = int(tag.interpreter[idx + 1:idx + 2])
        if sys.version_info.minor != supported_minor:
            return f"The python minor version is {sys.version_info.minor}, but the wheel only supports minor version {supported_minor}"

    # There should be no restriction on the platform:
    if tag.platform == "any":
        return ""
    pieces = tag.platform.split("_")
    if len(pieces) != 4:
        print("Unable to parse the platform tag")

    wheel_os_name = pieces[0]
    wheel_os_version_major = pieces[1]
    wheel_os_version_minor = pieces[2]
    cpu_architecture = pieces[3]
    if wheel_os_name == "macosx":
        if sys.platform != "darwin":
            return f"The wheel was build for macosx, but the current platform is {sys.platform}"

    if cpu_architecture != platform.machine():
        return f"The CPU architecture supported by the wheel is {cpu_architecture}, but the platform has architecture {platform.machine()}"

    os_major, os_minor, os_patch = platform.mac_ver()[0].split(".")
    if int(os_major) < int(wheel_os_version_major):
        return f"The operating system major version is {os_major}, but the wheel requires at least OS major version {wheel_os_version_major}"
    if int(os_major) == int(wheel_os_version_major):
        if int(os_minor) < int(wheel_os_version_minor):
            return f"The operating system minor version is {os_minor}, but the wheel requires at least OS major version {wheel_os_version_minor}"

    return ""


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Diagnoses why a wheel is unsupported on a particular platform."
    )
    parser.add_argument("wheelfile", type=Path, help="The name of the wheel file.")
    args = parser.parse_args()
    error_msg = diagnose_unsupported(args.wheelfile)
    if len(error_msg) > 0:
        print(
            f"ERROR: {args.wheelfile} is not supported on this platform. Reason: {error_msg}"
        )
    else:
        print(f"{args.wheelfile} should be supported on your platform!")
