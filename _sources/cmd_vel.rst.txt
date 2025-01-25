cmd_vel Module
==============

// This section documents the cmd_vel module, which includes functions for controlling the robot's velocity.

// Provide an overview of the cmd_vel module here.

LinearController
----------------
Controls the linear and angular velocity of the robot to reach a target position and orientation.

.. cpp:function:: void LinearController(const OdomData* odomData, double& linear_velocity, double& angular_velocity)

   Controls the linear and angular velocity of the robot to reach a target position and orientation.

   :param odomData: Pointer to the current odometry data.
   :type odomData: const OdomData*
   :param linear_velocity: Reference to the linear velocity to be controlled.
   :type linear_velocity: double&
   :param angular_velocity: Reference to the angular velocity to be controlled.
   :type angular_velocity: double&

   :returns: None

SendCmdVel
----------
Sends velocity commands to the robot via a socket connection.

.. cpp:function:: void SendCmdVel(int port)

   Sends velocity commands to the robot via a socket connection.

   :param port: The port number to connect to the server.
   :type port: int