# arlobot_psoc
Contains the Psoc4 project files for the Arlobot on Raspberry Pi

This was my first attempt using a Raspberry Pi and Psoc4.  However, I found that the robot required more resources than were available on the Psoc4.  I did, for a time, attempt to stack two Psoc4 development boards on top of the Raspberry Pi but that design be cause unusable before of the need to communication between the boards.  I have since re-architected the solution to use the Sparkfun Freesoc which uses the Psoc5 and has all of the necessary resources and room to grow.

For those interested, see the arlobot_freesoc repo.
