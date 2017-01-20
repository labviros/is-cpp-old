#include <boost/program_options.hpp>
#include <iostream>
#include <armadillo>
#include <cmath>
#include <is/is.hpp>
#include <is/msgs/robot.hpp>

namespace po = boost::program_options;
using namespace std;
using namespace arma;
using namespace is::msg::robot;

constexpr double pi() { return atan(1)*4; }
auto deg2rad = [](double deg){return deg*(pi()/180.0);};
auto rad2deg = [](double rad){return rad*(180.0/pi());};

int main(int argc, char* argv[]) {
  std::string uri;
  std::string name {"controller"};

  po::options_description description("Allowed options");
  auto&& options = description.add_options();
  options("help,", "show available options");
  options("uri,u", po::value<std::string>(&uri)->default_value("amqp://localhost"), "broker uri");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << description << std::endl;
    return 1;
  }

  auto is = is::connect(uri);

  auto thread = is::advertise(uri, name, {
      {
        "final_position",
        [&](is::Request request) -> is::Reply {
            FinalPosition req = is::msgpack<FinalPosition>(request);
            //if () {
            //    // request odometry
            //    
            //}
            double x_til = req.desired.x - req.current.x;
            double y_til = req.desired.y - req.current.y;
            
            mat invA(2,2);
            invA.at(0,0) = cos(deg2rad(req.current.th));
            invA.at(0,1) = sin(deg2rad(req.current.th));
            invA.at(1,0) = -(1.0/req.center_offset)*sin(deg2rad(req.current.th));
            invA.at(1,1) =  (1.0/req.center_offset)*cos(deg2rad(req.current.th));

            mat C(2,1);
            C.at(0,0) = req.max_vel_x*tanh((req.gain_x/req.max_vel_x)*x_til);
            C.at(1,0) = req.max_vel_y*tanh((req.gain_y/req.max_vel_y)*y_til);
            
            mat vels_vec = invA*C;

            Velocities vels {vels_vec.at(0), rad2deg(vels_vec.at(1))};
            is::logger()->info("(v,w)=({},{})", vels.v, vels.w);
            return is::msgpack(vels);
        }
      }
   });

   thread.join();
  return 0;
}