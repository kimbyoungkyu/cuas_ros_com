#include <rclcpp/rclcpp.hpp>
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
class CUASPublisher : public rclcpp::Node
{
public:
	explicit CUASPublisher() : Node("cuas_msg_publisher")
	{
		RCLCPP_INFO(this->get_logger(), "CUAS Publisher initialized");
	}

private:
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