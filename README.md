# PX4-ROS2 bridge

[![GitHub license](https://img.shields.io/github/license/PX4/px4_ros_com.svg)](https://github.com/PX4/px4_ros_com/blob/master/LICENSE) [![GitHub (pre-)release](https://img.shields.io/github/release-pre/PX4/px4_ros_com.svg)](https://github.com/PX4/px4_ros_com/releases/tag/beta) [![DOI](https://zenodo.org/badge/142936318.svg)](https://zenodo.org/badge/latestdoi/142936318) [![Build and Test package](https://github.com/PX4/px4_ros_com/workflows/Build%20and%20Test%20package/badge.svg?branch=master)](https://github.com/PX4/px4_ros_com/actions)

[![Discord Shield](https://discordapp.com/api/guilds/1022170275984457759/widget.png?style=shield)](https://discord.gg/dronecode)

This package provides example nodes for exchanging data and commands between ROS2 and PX4.
It also provides a [library](./include/px4_ros_com/frame_transforms.h) to ease the conversion between ROS2 and PX4 frame conventions.
It has a straight dependency on the [`px4_msgs`](https://github.com/PX4/px4_msgs) package.

## Install, build and usage

Check the [uXRCE-DDS](https://docs.px4.io/main/en/middleware/uxrce_dds.html) and the [ROS2 Interface](https://docs.px4.io/main/en/ros/ros2_comm.html) sections on the PX4 Devguide for details on how to install the required dependencies, build the package and use it.

## Bug tracking and feature requests

Use the [Issues](https://github.com/PX4/px4_ros_com/issues) section to create a new issue. Report your issue or feature request [here](https://github.com/PX4/px4_ros_com/issues/new).

## Questions and troubleshooting

Reach the PX4 development team on the [PX4 Discord Server](https://discord.gg/dronecode).


## License & Attribution

This repository, `cuas_ros_com`, is an extension and modifications based on the official open-source [px4_ros_com](https://github.com/PX4/px4_ros_com) repository.

* **Original Software:** `px4_ros_com` (Copyright (c) 2020-2026, PX4 Development Team. All rights reserved.)
* **Original License:** BSD 3-Clause License
* **Modifications:** Custom CUAS messages (`cuas_msgs`) integration and C2 (Command and Control) system communication extensions are added by Kim Byoung-kyu.

This project inherits the **BSD 3-Clause License**. You may reuse, modify, and distribute this software under the terms and conditions of the license. For the full license text, please refer to the `LICENSE` file in this repository.

---

본 저장소는 오픈소스 프로젝트인 `px4_ros_com`을 기반으로 안티드론(Counter-UAS) 및 C2 지휘통제 시스템 연동 기능을 확장하기 위해 포크 및 수정된 프로젝트입니다. 원저작자인 PX4 개발팀의 BSD 3-Clause 라이선스 규정을 준수하며, 추가된 커스텀 확장 기능 역시 동일한 라이선스 하에 관리됩니다.