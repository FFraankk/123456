#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/int32.hpp"
#include <sstream>  // 用于字符串流
#include <vector>

class MySerialNode : public rclcpp::Node
{
public:
  MySerialNode() : Node("my_serial_node")
  {
    // 创建15个话题发布者
    pub_latitude = create_publisher<std_msgs::msg::Float64>("latitude", 10);
    pub_longitude = create_publisher<std_msgs::msg::Float64>("longitude", 10);
    pub_motor_speed_0 = create_publisher<std_msgs::msg::Int32>("motor_speed_0", 10);
    pub_motor_speed_1 = create_publisher<std_msgs::msg::Int32>("motor_speed_1", 10);
    pub_motor_speed_2 = create_publisher<std_msgs::msg::Int32>("motor_speed_2", 10);
    pub_motor_speed_3 = create_publisher<std_msgs::msg::Int32>("motor_speed_3", 10);
    pub_real_vcx = create_publisher<std_msgs::msg::Float32>("real_vcx", 10);
    pub_real_w = create_publisher<std_msgs::msg::Float32>("real_w", 10);
    pub_gyro_z = create_publisher<std_msgs::msg::Float32>("gyro_z", 10);
    pub_roll = create_publisher<std_msgs::msg::Float32>("roll", 10);
    pub_pitch = create_publisher<std_msgs::msg::Float32>("pitch", 10);
    pub_yaw = create_publisher<std_msgs::msg::Float32>("yaw", 10);
    pub_linear_acc_x = create_publisher<std_msgs::msg::Float32>("linear_acc_x", 10);
    pub_gps_diff_x = create_publisher<std_msgs::msg::Float32>("gps_diff_x", 10);
    pub_gps_diff_y = create_publisher<std_msgs::msg::Float32>("gps_diff_y", 10);

    // 初始化串口接收逻辑，例如使用串口库
    // 在这里你可以使用你常用的串口通信库，例如boost::asio或者serial库等
    // 以下是一个简单的使用boost::asio的示例
    // ...

    // 设置串口数据回调函数
    serial_data_subscription = create_subscription<std_msgs::msg::String>(
      "serial_data", 10,
      [this](const std_msgs::msg::String::SharedPtr msg) {
        // 解析串口数据，并发布到相应话题
        parseAndPublish(msg->data);
      });
  }

private:
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pub_latitude;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr pub_longitude;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_motor_speed_0;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_motor_speed_1;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_motor_speed_2;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_motor_speed_3;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_real_vcx;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_real_w;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_gyro_z;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_roll;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_pitch;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_yaw;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_linear_acc_x;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_gps_diff_x;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_gps_diff_y;

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr serial_data_subscription;

  void parseAndPublish(const std::string &data)
  {
    // 解析串口数据，假设数据格式如下：
    // "%lf,%lf,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n"

    std::istringstream iss(data);
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(iss, token, ',')) {
      tokens.push_back(token);
    }

    if (tokens.size() == 15) {
      try {
        // 将解析后的数据转换为相应的类型
        double latitude = std::stod(tokens[0]);
        double longitude = std::stod(tokens[1]);
        int motor_speed[4] = {std::stoi(tokens[2]), std::stoi(tokens[3]), std::stoi(tokens[4]), std::stoi(tokens[5])};
        float real_vcx = std::stof(tokens[6]);
        float real_w = std::stof(tokens[7]);
        float gyro_z = std::stof(tokens[8]);
        float roll = std::stof(tokens[9]);
        float pitch = std::stof(tokens[10]);
        float yaw = std::stof(tokens[11]);
        float linear_acc_x = std::stof(tokens[12]);
        float gps_diff_x = std::stof(tokens[13]);
        float gps_diff_y = std::stof(tokens[14]);

        // 发布到相应话题
        pub_latitude->publish(latitude);
        pub_longitude->publish(longitude);
        pub_motor_speed_0->publish(motor_speed[0]);
        pub_motor_speed_1->publish(motor_speed[1]);
        pub_motor_speed_2->publish(motor_speed[2]);
        pub_motor_speed_3->publish(motor_speed[3]);
        pub_real_vcx->publish(real_vcx);
        pub_real_w->publish(real_w);
        pub_gyro_z->publish(gyro_z);
        pub_roll->publish(roll);
        pub_pitch->publish(pitch);
        pub_yaw->publish(yaw);
        pub_linear_acc_x->publish(linear_acc_x);
        pub_gps_diff_x->publish(gps_diff_x);
        pub_gps_diff_y->publish(gps_diff_y);

      } catch (const std::exception &e) {
        RCLCPP_ERROR(get_logger(), "Error parsing serial data: %s", e.what());
      }
    } else {
      RCLCPP_ERROR(get_logger(), "Invalid number of fields in serial data");
    }
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MySerialNode>());
  rclcpp::shutdown();
  return 0;
}
