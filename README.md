# Mastergard_Safety_Brake


Please use PlatformIO extension on Visual-Studio Code to open the project.
This branch uses Non Blocking timers to operate the circuit.
Use code from this branch to implement added features to run in main.cpp without delays from circuit operation. 
Going forward, commits must be made to this branch as this is the most up to date with all requested features.

**PLEASE CONTINUE READING FOR AN IMPORTANT ASPECT**

The code determines whether to operate the circuit based on the emergencyFilter() function.
This function uses two values of acceleration sourced from the accelerometer 25 ms apart to determine whether brake light flashing condition is met or not.
![image](https://user-images.githubusercontent.com/66683154/132103031-c6ae0f79-b381-4203-ab5b-07a7af7ef639.png)
**(inside helper_functions.cpp)**
It is able to do this by  - 
1. Detecting if deceleration occurred - i.e. if the sign of acceleration has changed
2. If the above condition is true - check if magnitude of deceleration is greater than or equal to threshold

Ideally this function should work fine. However, there might be a small chance that this function misses vital data points which might cause it to work incorrectly.
In such a case, detecting deceleration (i.e. sign change) would be futile and a higher importance must be given to whether any acceleration detected by the circuit
is greater than or equal to threshold. This approach works because the circuit is only switched on when the brake is pushed and therefore any acceleration detected
by the circuit must be deceleration.

For this reason a second function has been defined - emergencyFilterSimplified()
![image](https://user-images.githubusercontent.com/66683154/132103272-409732b0-77a7-4812-a364-3985f4f81778.png)
**(inside helper_functions.cpp)**

This function remains commented inside the code and the header files.
If it is decided that the initial function does not give desire response, all known occurences of emergencyFilter() must be commented and all known occurences
of emergencyFilterSimplified must be uncommented.

![image](https://user-images.githubusercontent.com/66683154/132103299-e28f9b36-5be1-40ce-9a55-4756ecc352b9.png)
**(inside helper_functions.h)**
![image](https://user-images.githubusercontent.com/66683154/132103312-f1e3cb7f-e0ce-4e47-99e7-6f364345aca8.png)
**(inside main.cpp)**
