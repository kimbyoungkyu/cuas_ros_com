#include <rclcpp/rclcpp.hpp>
#include <px4_msgs/msg/sensor_gps.hpp>
#include "cuas_msgs/msg/c2_command.hpp"
#include "cuas_msgs/msg/engagement_result.hpp"
#include "cuas_msgs/msg/fault_report.hpp"
#include "cuas_msgs/msg/intercept_mission.hpp"
#include "cuas_msgs/msg/interceptor_status.hpp"
#include "cuas_msgs/msg/intercept_progress.hpp"
#include "cuas_msgs/msg/mission_ack.hpp"
#include "cuas_msgs/msg/target_track.hpp"

/**
 * @brief CUAS Listener Node
 */
class CUASListener : public rclcpp::Node
{
public:
	explicit CUASListener() : Node("cuas_listener")
	{
		rmw_qos_profile_t qos_profile = rmw_qos_profile_sensor_data;
		auto qos = rclcpp::QoS(rclcpp::QoSInitialization(qos_profile.history, 5),qos_profile);
		target_track_sub_ =	this->create_subscription<cuas_msgs::msg::TargetTrack>("/cuas/c2/target_track",qos,std::bind(&CUASListener::targetTrackCallback,this,std::placeholders::_1));
		RCLCPP_INFO(this->get_logger(), "CUAS Listener initialized");
	}

private:
	/**
	 * @brief TargetTrack callback
	 */
	void targetTrackCallback(const cuas_msgs::msg::TargetTrack::SharedPtr msg)
	{
		std::string track_state_str = getTrackStateString(msg->track_state);
		RCLCPP_INFO(
			this->get_logger(),
			"\n"
			"================ TARGET TRACK ================\n"
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
	 * @brief Track state to string
	 */
	std::string getTrackStateString(uint8_t state)
	{
		switch (state)
		{
		case cuas_msgs::msg::TargetTrack::UNKNOWN:
			return "UNKNOWN";

		case cuas_msgs::msg::TargetTrack::DETECTED:
			return "DETECTED";

		case cuas_msgs::msg::TargetTrack::TRACKING:
			return "TRACKING";

		case cuas_msgs::msg::TargetTrack::LOST:
			return "LOST";

		case cuas_msgs::msg::TargetTrack::CONFIRMED:
			return "CONFIRMED";

		default:
			return "INVALID";
		}
	}

private:
	rclcpp::Subscription<px4_msgs::msg::SensorGps>::SharedPtr subscription_;

	rclcpp::Subscription<cuas_msgs::msg::TargetTrack>::SharedPtr
		target_track_sub_;
};

int main(int argc, char *argv[])
{
	std::cout << "Starting CUAS msg listener node..." << std::endl;
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<CUASListener>());
	rclcpp::shutdown();
	return 0;
}