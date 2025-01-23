laser_data Module
=================

// This section documents the laser_data module, which includes functions for reading the laserscans data as well as visualizing the data in a map.

extract_json
----------------
Controls the linear and angular velocity of the robot to reach a target position and orientation.

.. cpp:function::std::string extract_json(const std::string &text)

   Controls the linear and angular velocity of the robot to reach a target position and orientation.

   :param odomData: Pointer to the current odometry data.
   :type odomData: const OdomData*
   :param linear_velocity: Reference to the linear velocity to be controlled.
   :type linear_velocity: double&
   :param angular_velocity: Reference to the angular velocity to be controlled.
   :type angular_velocity: double&

   :return: None
