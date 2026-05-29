/**
 * @brief C-UAS Interceptor Report Publisher Example
 *
 * 본 예제는 C-UAS 체계에서 요격체(Interceptor)가
 * C2(Command & Control) 시스템으로 보고하는 ROS2 메시지를
 * 주기적으로 발행하는 테스트/시뮬레이션용 Publisher 노드이다.
 *
 * 발행 메시지:
 * - InterceptorStatus   : 요격체 상태, 위치, 속도, 배터리, Arm 상태
 * - InterceptProgress   : 요격 임무 진행 단계, 표적 거리, 상대 속도
 * - MissionAck          : C2 명령 수신 및 처리 결과
 * - EngagementResult    : 요격 결과 및 교전 요약
 * - FaultReport         : 요격체 고장/경고 정보
 *
 * 본 예제는 실제 비행 제어를 수행하지 않으며,
 * C-UAS 메시지 인터페이스 검증을 위한 샘플 코드이다.
 */

#include <chrono>
#include <iostream>
#include <memory>

#include <rclcpp/rclcpp.hpp>

#include "cuas_msgs/msg/engagement_result.hpp"
#include "cuas_msgs/msg/fault_report.hpp"
#include "cuas_msgs/msg/intercept_progress.hpp"
#include "cuas_msgs/msg/interceptor_status.hpp"
#include "cuas_msgs/msg/mission_ack.hpp"

using namespace std::chrono_literals;

/**
 * @brief 요격체 보고 메시지 Publisher 노드
 */
class CUASPublisher : public rclcpp::Node
{
public:
	/**
	 * @brief Publisher와 Timer를 초기화한다.
	 */
	explicit CUASPublisher()
		: Node("cuas_msg_publisher")
	{
		interceptor_status_pub_ =
			this->create_publisher<cuas_msgs::msg::InterceptorStatus>(
				"/cuas/interceptor/status",
				10);

		intercept_progress_pub_ =
			this->create_publisher<cuas_msgs::msg::InterceptProgress>(
				"/cuas/interceptor/progress",
				10);

		mission_ack_pub_ =
			this->create_publisher<cuas_msgs::msg::MissionAck>(
				"/cuas/interceptor/ack",
				10);

		engagement_result_pub_ =
			this->create_publisher<cuas_msgs::msg::EngagementResult>(
				"/cuas/interceptor/result",
				10);

		fault_report_pub_ =
			this->create_publisher<cuas_msgs::msg::FaultReport>(
				"/cuas/interceptor/fault",
				10);

		timer_ =
			this->create_wall_timer(
				1000ms,
				std::bind(&CUASPublisher::timerCallback, this));

		RCLCPP_INFO(
			this->get_logger(),
			"CUAS Publisher initialized");
	}

private:
	/**
	 * @brief 주기적으로 테스트 메시지를 발행한다.
	 */
	void timerCallback()
	{
		publishInterceptorStatus();
		publishInterceptProgress();
		publishMissionAck();
		publishEngagementResult();
		publishFaultReport();
	}

	/**
	 * @brief 요격체 현재 상태 메시지를 발행한다.
	 *
	 * 위치, 속도, 배터리, Arm 상태, Offboard 상태 등을 포함한다.
	 */
	void publishInterceptorStatus()
	{
		cuas_msgs::msg::InterceptorStatus msg;

		msg.stamp = this->get_clock()->now();

		msg.interceptor_id = "INTERCEPTOR_001";
		msg.mission_id = "MISSION_001";

		msg.vehicle_state =
			cuas_msgs::msg::InterceptorStatus::ACTIVE;

		msg.latitude = 37.56650000;
		msg.longitude = 126.97800000;
		msg.altitude = 120.5f;

		msg.velocity_x = 18.5f;
		msg.velocity_y = 0.8f;
		msg.velocity_z = -1.2f;

		msg.battery_remaining = 86.5f;

		msg.armed = true;
		msg.offboard_enabled = true;
		msg.healthy = true;

		interceptor_status_pub_->publish(msg);

		RCLCPP_INFO(
			this->get_logger(),
			"Published InterceptorStatus");
	}

	/**
	 * @brief 요격 임무 진행 상태 메시지를 발행한다.
	 *
	 * 현재 임무 단계, 표적까지 거리, 상대 속도,
	 * 임무 경과 시간을 포함한다.
	 */
	void publishInterceptProgress()
	{
		cuas_msgs::msg::InterceptProgress msg;

		msg.stamp = this->get_clock()->now();

		msg.mission_id = "MISSION_001";
		msg.interceptor_id = "INTERCEPTOR_001";
		msg.target_id = "TARGET_001";

		msg.phase =
			cuas_msgs::msg::InterceptProgress::MIDCOURSE;

		msg.distance_to_target = 850.0f;
		msg.relative_speed = 72.5f;
		msg.mission_elapsed_sec = 15.3f;

		msg.status_text = "Midcourse guidance in progress";

		intercept_progress_pub_->publish(msg);

		RCLCPP_INFO(
			this->get_logger(),
			"Published InterceptProgress");
	}

	/**
	 * @brief C2 명령에 대한 응답 메시지를 발행한다.
	 *
	 * 명령 ID, 임무 ID, 승인 여부, 처리 결과 코드를 포함한다.
	 */
	void publishMissionAck()
	{
		cuas_msgs::msg::MissionAck msg;

		msg.stamp = this->get_clock()->now();

		msg.command_id = "CMD_001";
		msg.mission_id = "MISSION_001";
		msg.interceptor_id = "INTERCEPTOR_001";

		msg.accepted = true;

		msg.result_code =
			cuas_msgs::msg::MissionAck::OK;

		msg.message = "Mission command accepted";

		mission_ack_pub_->publish(msg);

		RCLCPP_INFO(
			this->get_logger(),
			"Published MissionAck");
	}

	/**
	 * @brief 요격 결과 메시지를 발행한다.
	 *
	 * 최종 요격 결과, 최종 거리, 교전 시간을 포함한다.
	 */
	void publishEngagementResult()
	{
		cuas_msgs::msg::EngagementResult msg;

		msg.stamp = this->get_clock()->now();

		msg.mission_id = "MISSION_001";
		msg.interceptor_id = "INTERCEPTOR_001";
		msg.target_id = "TARGET_001";

		msg.result =
			cuas_msgs::msg::EngagementResult::SUCCESS_SIM;

		msg.final_distance = 2.35f;
		msg.engagement_time_sec = 21.7f;

		msg.summary =
			"Target successfully intercepted in simulation";

		engagement_result_pub_->publish(msg);

		RCLCPP_INFO(
			this->get_logger(),
			"Published EngagementResult");
	}

	/**
	 * @brief 고장/경고 보고 메시지를 발행한다.
	 *
	 * 고장 코드, 고장 이름, 설명, 임무 중단 필요 여부를 포함한다.
	 */
	void publishFaultReport()
	{
		cuas_msgs::msg::FaultReport msg;

		msg.stamp = this->get_clock()->now();

		msg.interceptor_id = "INTERCEPTOR_001";
		msg.mission_id = "MISSION_001";

		msg.severity =
			cuas_msgs::msg::FaultReport::WARNING;

		msg.fault_code = 1001;
		msg.fault_name = "BatteryLow";
		msg.description =
			"Battery level is below warning threshold";

		msg.requires_abort = false;

		fault_report_pub_->publish(msg);

		RCLCPP_INFO(
			this->get_logger(),
			"Published FaultReport");
	}

private:
	rclcpp::Publisher<cuas_msgs::msg::InterceptorStatus>::SharedPtr
		interceptor_status_pub_;

	rclcpp::Publisher<cuas_msgs::msg::InterceptProgress>::SharedPtr
		intercept_progress_pub_;

	rclcpp::Publisher<cuas_msgs::msg::MissionAck>::SharedPtr
		mission_ack_pub_;

	rclcpp::Publisher<cuas_msgs::msg::EngagementResult>::SharedPtr
		engagement_result_pub_;

	rclcpp::Publisher<cuas_msgs::msg::FaultReport>::SharedPtr
		fault_report_pub_;

	rclcpp::TimerBase::SharedPtr timer_;
};

/**
 * @brief 프로그램 진입점
 */
int main(int argc, char* argv[])
{
	std::cout << "Starting CUAS msg publisher node..." << std::endl;

	setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<CUASPublisher>());
	rclcpp::shutdown();

	return 0;
}