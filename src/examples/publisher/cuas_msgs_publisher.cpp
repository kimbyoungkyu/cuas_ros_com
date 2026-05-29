#include <chrono>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include "cuas_msgs/msg/c2_command.hpp"
#include "cuas_msgs/msg/engagement_result.hpp"
#include "cuas_msgs/msg/fault_report.hpp"
#include "cuas_msgs/msg/intercept_mission.hpp"
#include "cuas_msgs/msg/interceptor_status.hpp"
#include "cuas_msgs/msg/intercept_progress.hpp"
#include "cuas_msgs/msg/mission_ack.hpp"
#include "cuas_msgs/msg/target_track.hpp"

using namespace std::chrono_literals;

/**
 * @brief CUAS Publisher Node
 */
class CUASPublisher : public rclcpp::Node
{
public:
	explicit CUASPublisher() : Node("cuas_msg_publisher")
	{
		// Publishers

		//요격체 -> C2
		interceptor_status_pub_ = this->create_publisher<cuas_msgs::msg::InterceptorStatus>("/cuas/interceptor/status", 10);
		intercept_progress_pub_ = this->create_publisher<cuas_msgs::msg::InterceptProgress>("/cuas/interceptor/progress", 10);
		mission_ack_pub_ = this->create_publisher<cuas_msgs::msg::MissionAck>("/cuas/interceptor/ack", 10);
		engagement_result_pub_ = this->create_publisher<cuas_msgs::msg::EngagementResult>("/cuas/interceptor/result", 10);
		fault_report_pub_ = this->create_publisher<cuas_msgs::msg::FaultReport>("/cuas/interceptor/fault", 10);



		
		
		//intercept_mission_pub_ = this->create_publisher<cuas_msgs::msg::InterceptMission>("/cuas/intercept/mission", 10);
		
		
		
		// Timer
		timer_ = this->create_wall_timer(1000ms,std::bind(&CUASPublisher::timerCallback, this));
		RCLCPP_INFO(this->get_logger(), "CUAS Publisher initialized");
	}

private:
	void timerCallback()
	{
		publishInterceptorStatus();
		publishInterceptProgress();
		publishMissionAck();
		publishEngagementResult();
		publishFaultReport();
		//publishInterceptMission();
		
		
		
	}

	/**
	 * @brief Publish EngagementResult
	 */
	void publishEngagementResult()
	{
		cuas_msgs::msg::EngagementResult msg;

		// 예시 값
		// 실제 msg 정의에 맞게 수정하세요.
		msg.result = true;

		engagement_result_pub_->publish(msg);

		RCLCPP_INFO(this->get_logger(),	"Published EngagementResult");
	}

	/**
	 * @brief Publish FaultReport
	 */
	void publishFaultReport()
	{
		cuas_msgs::msg::FaultReport msg;

		// 예시 값
		msg.fault_code = 1;
		msg.description = "Battery Low";

		fault_report_pub_->publish(msg);

		RCLCPP_INFO(this->get_logger(),	"Published FaultReport");
	}

	/**
	 * @brief Publish InterceptMission
	 */
	
	/*
	 void publishInterceptMission()
	{
		cuas_msgs::msg::InterceptMission msg;

		msg.mission_id = "MISSION_001";
		msg.target_id = "TARGET_001";

		intercept_mission_pub_->publish(msg);

		RCLCPP_INFO(this->get_logger(),"Published InterceptMission");
	}
*/
	/**
	 * @brief Publish InterceptorStatus
	 */
	void publishInterceptorStatus()
	{
		cuas_msgs::msg::InterceptorStatus msg;

		msg.interceptor_id = "INTERCEPTOR_001";
		//msg.status = "READY";

		interceptor_status_pub_->publish(msg);

		RCLCPP_INFO(this->get_logger(),"Published InterceptorStatus");
	}

	/**
	 * @brief Publish InterceptProgress
	 */
	void publishInterceptProgress()
	{
		cuas_msgs::msg::InterceptProgress msg;

		//msg.progress = 75.0f;

		intercept_progress_pub_->publish(msg);

		RCLCPP_INFO(this->get_logger(),"Published InterceptProgress");
	}

	/**
	 * @brief Publish MissionAck
	 */
	void publishMissionAck()
	{
		cuas_msgs::msg::MissionAck msg;

		msg.mission_id = "MISSION_001";
		msg.accepted = true;

		mission_ack_pub_->publish(msg);

		RCLCPP_INFO(this->get_logger(),"Published MissionAck");
	}

private:
	// Publishers
	rclcpp::Publisher<cuas_msgs::msg::EngagementResult>::SharedPtr engagement_result_pub_;

	rclcpp::Publisher<cuas_msgs::msg::FaultReport>::SharedPtr fault_report_pub_;

	//rclcpp::Publisher<cuas_msgs::msg::InterceptMission>::SharedPtr intercept_mission_pub_;

	rclcpp::Publisher<cuas_msgs::msg::InterceptorStatus>::SharedPtr interceptor_status_pub_;

	rclcpp::Publisher<cuas_msgs::msg::InterceptProgress>::SharedPtr intercept_progress_pub_;

	rclcpp::Publisher<cuas_msgs::msg::MissionAck>::SharedPtr mission_ack_pub_;

	// Timer
	rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
	std::cout << "Starting CUAS msg publisher node..." << std::endl;
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<CUASPublisher>());
	rclcpp::shutdown();
	return 0;
}