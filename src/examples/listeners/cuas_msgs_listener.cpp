/**
 * @brief C-UAS C2 Command Listener Example
 *
 * 본 예제는 Counter-UAS(C-UAS) 체계에서
 * C2(Command & Control) 시스템이 요격체(Interceptor)에게
 * 전달하는 명령 및 임무 메시지를 ROS2 토픽으로 수신하고,
 * 수신된 내용을 로그로 확인하는 방법을 보여준다.
 *
 * 주요 기능:
 * - C2Command 메시지 수신
 * - InterceptMission 메시지 수신
 * - TargetTrack 메시지 수신
 * - C2 명령 정보 출력
 * - 요격 임무 정보 출력
 * - 표적 추적 정보 출력
 *
 * 본 예제는 실제 비행 제어를 수행하지 않으며,
 * C2와 요격체 간 메시지 인터페이스 검증을 위한
 * 테스트/디버깅용 Listener 노드이다.
 */

#include <iostream>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <rmw/qos_profiles.h>

#include "cuas_msgs/msg/c2_command.hpp"
#include "cuas_msgs/msg/intercept_mission.hpp"
#include "cuas_msgs/msg/target_track.hpp"

/**
 * @brief C-UAS Listener Node
 *
 * C2 시스템에서 발행하는 명령, 임무, 표적 추적 메시지를
 * 수신하여 터미널 로그로 출력하는 테스트용 ROS2 노드이다.
 */
class CUASListener : public rclcpp::Node
{
public:
	/**
	 * @brief Subscriber 초기화
	 *
	 * 구독 토픽:
	 * - /cuas/c2/command
	 * - /cuas/c2/mission
	 * - /cuas/c2/target_track
	 */
	explicit CUASListener()
		: Node("cuas_listener")
	{
		rmw_qos_profile_t qos_profile = rmw_qos_profile_sensor_data;

		auto qos = rclcpp::QoS(
			rclcpp::QoSInitialization(qos_profile.history, 5),
			qos_profile);

		c2_command_sub_ =
			this->create_subscription<cuas_msgs::msg::C2Command>(
				"/cuas/c2/command",
				qos,
				std::bind(
					&CUASListener::c2CommandCallback,
					this,
					std::placeholders::_1));

		intercept_mission_sub_ =
			this->create_subscription<cuas_msgs::msg::InterceptMission>(
				"/cuas/c2/mission",
				qos,
				std::bind(
					&CUASListener::c2MissionCallback,
					this,
					std::placeholders::_1));

		target_track_sub_ =
			this->create_subscription<cuas_msgs::msg::TargetTrack>(
				"/cuas/c2/target_track",
				qos,
				std::bind(
					&CUASListener::targetTrackCallback,
					this,
					std::placeholders::_1));

		RCLCPP_INFO(
			this->get_logger(),
			"CUAS Listener initialized");
	}

private:
	/**
	 * @brief C2 명령 메시지 수신 콜백
	 *
	 * C2가 요격체에게 전달한 명령 정보를 출력한다.
	 */
	void c2CommandCallback(
		const cuas_msgs::msg::C2Command::SharedPtr msg)
	{
		const std::string command_type_str =
			getCommandTypeString(msg->command_type);

		RCLCPP_INFO(
			this->get_logger(),
			"\n"
			"================ C2 COMMAND ================\n"
			"Stamp       : %d.%09u\n"
			"Command ID  : %s\n"
			"Mission ID  : %s\n"
			"Interceptor : %s\n"
			"Target ID   : %s\n"
			"CommandType : %s (%d)\n"
			"Reason      : %s\n"
			"============================================",
			msg->stamp.sec,
			msg->stamp.nanosec,
			msg->command_id.c_str(),
			msg->mission_id.c_str(),
			msg->interceptor_id.c_str(),
			msg->target_id.c_str(),
			command_type_str.c_str(),
			msg->command_type,
			msg->reason.c_str());
	}

	/**
	 * @brief 요격 임무 메시지 수신 콜백
	 *
	 * C2가 생성한 요격 임무 정보와 포함된 표적 정보를 출력한다.
	 */
	void c2MissionCallback(
		const cuas_msgs::msg::InterceptMission::SharedPtr msg)
	{
		const std::string track_state_str =
			getTrackStateString(msg->target.track_state);

		RCLCPP_INFO(
			this->get_logger(),
			"\n"
			"================ INTERCEPT MISSION ================\n"
			"Stamp                     : %d.%09u\n"
			"\n"
			"Mission ID                : %s\n"
			"Interceptor ID            : %s\n"
			"Target ID                 : %s\n"
			"\n"
			"---------------- Target ----------------\n"
			"Target ID                 : %s\n"
			"Target Stamp              : %d.%09u\n"
			"Latitude                  : %.8f\n"
			"Longitude                 : %.8f\n"
			"Altitude                  : %.2f m\n"
			"\n"
			"Velocity X                : %.2f m/s\n"
			"Velocity Y                : %.2f m/s\n"
			"Velocity Z                : %.2f m/s\n"
			"\n"
			"Heading                   : %.2f deg\n"
			"Confidence                : %.2f\n"
			"Track State               : %s (%d)\n"
			"\n"
			"---------------- Mission ----------------\n"
			"Max Speed                 : %.2f m/s\n"
			"Safe Altitude             : %.2f m\n"
			"Loiter Altitude           : %.2f m\n"
			"\n"
			"Launch Latitude           : %.8f\n"
			"Launch Longitude          : %.8f\n"
			"Launch Altitude           : %.2f m\n"
			"\n"
			"Home Latitude             : %.8f\n"
			"Home Longitude            : %.8f\n"
			"Home Altitude             : %.2f m\n"
			"\n"
			"Allow Terminal Phase      : %s\n"
			"Allow Auto Return         : %s\n"
			"Abort On Lost Target      : %s\n"
			"===================================================",
			msg->stamp.sec,
			msg->stamp.nanosec,

			msg->mission_id.c_str(),
			msg->interceptor_id.c_str(),
			msg->target_id.c_str(),

			msg->target.target_id.c_str(),
			msg->target.stamp.sec,
			msg->target.stamp.nanosec,
			msg->target.latitude,
			msg->target.longitude,
			msg->target.altitude,

			msg->target.velocity_x,
			msg->target.velocity_y,
			msg->target.velocity_z,

			msg->target.heading,
			msg->target.confidence,
			track_state_str.c_str(),
			msg->target.track_state,

			msg->max_speed,
			msg->safe_altitude,
			msg->loiter_altitude,

			msg->launch_latitude,
			msg->launch_longitude,
			msg->launch_altitude,

			msg->home_latitude,
			msg->home_longitude,
			msg->home_altitude,

			toBoolString(msg->allow_terminal_phase),
			toBoolString(msg->allow_auto_return),
			toBoolString(msg->allow_abort_on_lost_target));
	}

	/**
	 * @brief 표적 추적 메시지 수신 콜백
	 *
	 * 센서/레이더 또는 C2에서 전달된 표적 추적 정보를 출력한다.
	 */
	void targetTrackCallback(
		const cuas_msgs::msg::TargetTrack::SharedPtr msg)
	{
		const std::string track_state_str =
			getTrackStateString(msg->track_state);

		RCLCPP_INFO(
			this->get_logger(),
			"\n"
			"================ TARGET TRACK ================\n"
			"Stamp       : %d.%09u\n"
			"Target ID   : %s\n"
			"Latitude    : %.8f\n"
			"Longitude   : %.8f\n"
			"Altitude    : %.2f m\n"
			"\n"
			"Velocity X  : %.2f m/s\n"
			"Velocity Y  : %.2f m/s\n"
			"Velocity Z  : %.2f m/s\n"
			"\n"
			"Heading     : %.2f deg\n"
			"Confidence  : %.2f\n"
			"Track State : %s (%d)\n"
			"==============================================",
			msg->stamp.sec,
			msg->stamp.nanosec,
			msg->target_id.c_str(),
			msg->latitude,
			msg->longitude,
			msg->altitude,
			msg->velocity_x,
			msg->velocity_y,
			msg->velocity_z,
			msg->heading,
			msg->confidence,
			track_state_str.c_str(),
			msg->track_state);
	}

	/**
	 * @brief C2Command command_type 값을 문자열로 변환
	 */
	std::string getCommandTypeString(uint8_t type) const
	{
		switch (type)
		{
		case cuas_msgs::msg::C2Command::ASSIGN_TARGET:
			return "AssignTarget";

		case cuas_msgs::msg::C2Command::PREPARE_INTERCEPT:
			return "PrepareIntercept";

		case cuas_msgs::msg::C2Command::AUTHORIZE_LAUNCH:
			return "AuthorizeLaunch";

		case cuas_msgs::msg::C2Command::START_INTERCEPT:
			return "StartIntercept";

		case cuas_msgs::msg::C2Command::UPDATE_TARGET:
			return "UpdateTarget";

		case cuas_msgs::msg::C2Command::UPDATE_MISSION:
			return "UpdateMission";

		case cuas_msgs::msg::C2Command::HOLD:
			return "Hold";

		case cuas_msgs::msg::C2Command::ABORT:
			return "Abort";

		case cuas_msgs::msg::C2Command::RETURN_HOME:
			return "ReturnHome";

		case cuas_msgs::msg::C2Command::LAND:
			return "Land";

		default:
			return "Unknown";
		}
	}

	/**
	 * @brief TargetTrack track_state 값을 문자열로 변환
	 */
	std::string getTrackStateString(uint8_t state) const
	{
		switch (state)
		{
		case cuas_msgs::msg::TargetTrack::UNKNOWN:
			return "Unknown";

		case cuas_msgs::msg::TargetTrack::DETECTED:
			return "Detected";

		case cuas_msgs::msg::TargetTrack::TRACKING:
			return "Tracking";

		case cuas_msgs::msg::TargetTrack::LOST:
			return "Lost";

		case cuas_msgs::msg::TargetTrack::CONFIRMED:
			return "Confirmed";

		default:
			return "Invalid";
		}
	}

	/**
	 * @brief bool 값을 로그 출력용 문자열로 변환
	 */
	const char* toBoolString(bool value) const
	{
		return value ? "true" : "false";
	}

private:
	rclcpp::Subscription<cuas_msgs::msg::C2Command>::SharedPtr
		c2_command_sub_;

	rclcpp::Subscription<cuas_msgs::msg::InterceptMission>::SharedPtr
		intercept_mission_sub_;

	rclcpp::Subscription<cuas_msgs::msg::TargetTrack>::SharedPtr
		target_track_sub_;
};

/**
 * @brief 프로그램 진입점
 */
int main(int argc, char* argv[])
{
	std::cout << "Starting CUAS msg listener node..." << std::endl;

	setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<CUASListener>());
	rclcpp::shutdown();

	return 0;
}