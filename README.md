# 1. Block Test

- [1. Block Test](#1-block-test)
- [2. Introduction](#2-introduction)
- [3. Installation](#3-installation)
- [4. Test writing](#4-test-writing)
  - [4.1. General Settings](#41-general-settings)
  - [4.2. Library Settings](#42-library-settings)
  - [4.3. Prerequisites](#43-prerequisites)
  - [4.4. Test list](#44-test-list)
  - [4.5. Finally the test](#45-finally-the-test)
    - [4.5.1. Generic commands](#451-generic-commands)
    - [4.5.2. Yarp commands](#452-yarp-commands)
    - [4.5.3. Yarp walking commands](#453-yarp-walking-commands)
  - [Test writing with scriptbuilder](#test-writing-with-scriptbuilder)
  - [4.6. Parametric test](#46-parametric-test)
  - [4.7. Using the parameter](#47-using-the-parameter)
  - [4.8. Examples](#48-examples)
- [5. Model settings](#5-model-settings)
- [6. Test execution](#6-test-execution)
- [7. Logging](#7-logging)
  - [7.1. Application logging](#71-application-logging)
  - [7.2. Sensors logging](#72-sensors-logging)
  - [7.3. Plot logging](#73-plot-logging)
  - [7.4. Report](#74-report)
  - [7.5. Timeout](#75-timeout)
- [8. Specific library writing](#8-specific-library-writing)
- [9. Bugs](#9-bugs)
- [10. Authors](#10-authors)

# 2. Introduction

The application provides functionalities for developing and running not regression tests in a likely natural language as close as possibile to test case writing.
The test philosopy is to divide a test into elementary blocks. The
blocks can be used to build different tests (See Figure below).
<br/><br/>


![alt text](img/img001.png "The tests are made by elementary blocks.")
<br/><br/>

# 3. Installation

TODO     

# 4. Test writing

The starting point for writing a test is the file ./test/test.xml

```xml
    <testlist repetitions="1">

   <settings robotname="icubSim" realrobot="false" onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce" netclock="true" 
            neverexecutecommands="" logseverity="debug" loggingtime="0.01"  tablename="test/tables/main.tab" 
            waitcommand="yarpwait" nowcommand="yarpnow" loggingcommand="infologgeryarp"/> <!--loggingtime in sec-->

    <prerequisite enabled="true" command="yarpserver" waitafter="5000" param="--silent" prefix="" kill="true"/>
    <prerequisite enabled="false" command="gzserver" waitafter="5000" param="--verbose -e ode --profile ode_default -slibgazebo_yarp_clock.so icub.world" prefix="" kill="true"/>
    <prerequisite enabled="true" command="gzserver" waitafter="5000" param="--verbose -e ode --profile ode_default -slibgazebo_yarp_clock.so icub_fixed.world" prefix="" kill="true"/>
    <prerequisite enabled="false" command="gzclient" waitafter="5000" param="" prefix="" kill="true"/>


    <!--Libraries-->
    <library enabled="true" path="genericactiondepotlib/libgenericactiondepot.so" name="genericactiondepot" note="System generic action library"/>
    <library enabled="true" path="yarpactiondepotlib/libyarpactiondepot.so" name="yarpactiondepot" note="Yarp action library"/>

    <!--Libraries settings-->
    <librarysettings enabled="true" name="genericactiondepot"/>
    <librarysettings enabled="true" name="yarpactiondepot" wrappername="/right_leg /left_leg /torso /head /right_arm /left_arm"/>
   
    
    <!--**************************-->
    <!--**********Tests***********-->
    <!--**************************-->

    <!--ICub pos && directpos-->
    <test file="test//0001.xml" repetitions="2" name="ICub right ankle roll move" note="xxx" code="0001" version="1" loggingtype="position" loggingwrappername="/right_leg" loggingpart="r_ankle_roll r_ankle_pitch"/>

    </testlist>   
```

This file contains:
 * the simulation general settings
 * the used libraries
 * the prerequisites
 * the tests link list

## 4.1. General Settings

```xml
<settings robotname="icubSim" realrobot="false" onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce" netclock="true" 
            neverexecutecommands="" logseverity="debug" loggingtime="0.01"  tablename="test/tables/main.tab" 
            waitcommand="yarpwait" nowcommand="yarpnow" loggingcommand="infologgeryarp"/> <!--loggingtime in sec-->
```

| Param name | Default value | Comment |
|------------|---------------|---------|
| robotname             | icubSim    | Robot name to be used  |
| realrobot             | false     | Indicates if it is a real robot in test, or it is a Gazebo simulation |
| onlysimcommands       | yarpreset applyForce| Indicates which are the commands to be executed only in simulation |
| simclock              | true     | Indicates if the network clock should be used. In the case it is used the one on "/clock" port  |
| neverexecutecommands  | ---   | Indicates which are the commands not to be executed.        |
| logseverity           | info     | Indicates the severity to be logged in log.log   |
| loggingtime           | 0.01      | Indicates the time in seconds for logging joint info if are requested in test. |
| tablename             | test/tables/main.tab       | Indicates the table name for parametric value|
| waitcommand           | yarpwait      | Indicates the command blocks to be used for wait  |
| nowcommand            | yaronow       | Indicates the command blocks to be used for now|
| loggingcommand        | infologgeryarp    | Indicates the command blocks to be used for logging   |

## 4.2. Library Settings
In this section it is possible to specify the plugin library to be used.

```xml
<library enabled="true" path="genericactiondepotlib/libgenericactiondepot.so" name="genericactiondepot" note="System generic action library"/>
<library enabled="true" path="yarpactiondepotlib/libyarpactiondepot.so" name="yarpactiondepot" note="Yarp action library"/>
```

| Param name | Default value | Comment |
|------------|---------------|---------|
| ebabled             | true    | If the library will be loaded  |
| path             | ---     | Librari .so file path |
| name       | ---| library tag name |
| note              | ---     | Explanation notes  |

In this section it is also possible to specify the plugin library settings.

```xml
<!--Libraries settings-->
<librarysettings enabled="true" name="genericactiondepot"/>

<librarysettings enabled="true" name="yarpactiondepot" wrappername="/right_leg /left_leg /torso /head /right_arm /left_arm"/>
```

| Param name | Default value | Comment |
|------------|---------------|---------|
| ebabled             | true    | If these settings will be loaded  |
| name       | ---| library tag name |
| ...              | ...     | ...  |


## 4.3. Prerequisites

The prerequisites are applications to be executed before the tests, if
necessary with parameters.

```xml
 <prerequisite enabled="true" command="gzserver" waitafter="5000" param="--verbose -e ode --profile ode_default -slibgazebo_yarp_clock.so icub_fixed.world" prefix="" kill="true"/>
```
| Param name | Default value | Comment |
|------------|---------------|---------|
| ebabled             | true    | If the prerequisite will be loaded  |
| command             | ---     | Command to be executed. |
| waitafter       | 5000| Time to wait after command execution. |
| param              | ---     | Command parameters.  |
| prefix              | ---     | Command prefixes.  |
| kill              | true     | The current prerequisite will be killed at the end of the tests.  |


## 4.4. Test list

The test list include all the tests written. The test list, basically,
list the tests together with the file in which the test has been written.

```xml
<test file="test//0001.xml" repetitions="2" name="ICub right ankle  roll move" note="xxx" code="0001" version="1" loggingtype="position" loggingwrappername="/right_leg" loggingpart="r_ankle_roll r_ankle_pitch"/>

<test file="test//0100.xml" repetitions="2" name="ICub right ankle roll pwm injection" note="xxx" code="0100" version="1" loggingtype="position" loggingwrappername="/right_leg" loggingpart="r_ankle_roll r_ankle_pitch"/>

<test file="test//0110.xml" repetitions="0" name="ICub right ankle roll pwmtrain injection" note="xxx" code="0110" version="1" loggingtype="" loggingwrappername="" loggingpart=""/>

```

| Param name | Default value | Comment |
|------------|---------------|---------|
| file             | ---    | File in which the test is written.  |
| repetitions             | 1     | How many times the test is repeated. |
| name       | ---| Test name. |
| note              | ---     | Test description.  |
| code              | ---     | Numeric code for identify the test, could be related to test case.  |
| version              | ---     | Test version.  |
| loggingtype              | ---     | Indicates what kind of logging you need. For now it can be "position", "com" or both.  |
| loggingpart             | ---     | Joints name to be logged.  |
| loggingwrappername             | ---     | The wrapper that controls the joints. Note that for now is possibile to specify just one wrapper.  |

## 4.5. Finally the test

The test is written in a separate file. Here is shown an example of a
simple test.
```xml
<testbody>
    <command name="yarpreset" repetitions="1" wait="0" reporterror="true"/>
   
   <command name="yarpsenddirectpos" wrappername="/right_leg" jointname="r_ankle_roll" degree="20" repetitions="1" wait="0" reporterror="true"/>    
</testbody>
```  

The test is composed by commands and each command has a parameters list.

The common parameters are shown in the following table.

| Param name | Default | Comment |
|------------|---------------|---------|
| name             | ---    | Command name.  |
| repetitions             | 1     | How many times the command is repeated. |
| wait       | 0| Wait time ad the end of the command execution in seconds. |
| reporterror              | true     | If the command should report error if necessary.  |

### 4.5.1. Generic commands
These commands are contained in the generic command library.

-   **wait**

    ```xml
        <command name="yarpwait" seconds="5" repetitions="1" wait="0" reporterror="true"/>   
    ```

    The parameter in seconds, is the wait time in seconds. Could be a double 0.001 means 1 msec.


-   **nop**

    ```xml
    <command name="nop" fixvalue="" tables="kp" printerror="true" printtestinfo="true" justonetime="false" repetitions="1" wait="0" reporterror="true"/>
    ```

    The nop command is used to print log on the plot.log file.

-   **updatefile**     

    ```xml
        <command    name="updatefile" 
                sourcefile="./walkingV2PIDparameters.ini" 
                destinationfile="./test/walkingV2PIDparameters.ini" 
                repetitions="1" 
                wait="0"
                reporterror="true">
    </command>
    ```

    The command copy and update the specified file.

-   **execute**

    ```xml       
	<command    name="execute" 
                command="icubWalking" 
                param="--gazeboClock --MPC::solver_name mumps --IK::solver_name mumps" 
                prefix="" 
                waitafter="2"
                kill="no"
                repetitions="1" 
                wait="0"
                reporterror="true">
    </command>
    ```

    The command execute the specified application.


### 4.5.2. Yarp commands
These commands are contained in the yarp command library.

-   **yarpreset**

    Reset the robot to the original frame position and pose.

    ```xml
        <command name="yarpreset" repetitions="1" wait="0" reporterror="true"/>
    ```

-   **yarpsendpwm**

    Send the pwm value to a specific jointname. The pwm profile can be: sin/const/stepwave

    ```xml
        <command name="yarpsendpwm" profile="sin" frequency="1" dutycycle="20" time="20" jointname="L_AK_R" repetitions="1" wait="0" reporterror="true"/>
    ```


-   **yarpsendposdirect**

    Send the position to a specific jointname

    ```xml
        <command name="yarpsenddirectpos" jointname="L_AK_R" degree="-30" repetitions="1" wait="0" reporterror="true"/>
    ```

-   **yarpsendpos**

    Send the position to a specific jointname using a minimum jerk trajectory.

    ```xml
        <command name="yarpsendpos" jointname="L_AK_R" degree="0" velocity="20" repetitions="1" wait="0" reporterror="true"/>
    ```

-   **yarpcheckrobot**

    ```xml
        <command name="checkRobot" repetitions="1" wait="0" reporterror="true"/>
    ```
    Check if joints are present.

-   **yarpcheckjointposition**

    Check if the joint position is correct.


    ```xml
        <command name="yarpcheckjointposition" wrappername="/right_leg" jointname="r_ankle_pitch" tolerance="1" expectedvalue="15" repetitions="1" wait="0" reporterror="true"/>
    ``` 

-   **yarpcheckrobotisvertical**

    Check if robot is vertical.

    ```xml
        <command name="yarpcheckrobotisvertical" repetitions="1" wait="0" reporterror="true"/>
    ``` 
-   **yarpsendpwmtrain**

    ```xml
        <command name="yarpsendpwmtrain" dutycycle="20" maxposition="20" minposition="-20" cycletime="10" cyclesleep="8" jointname="L_AK_R" repetitions="1" wait="0" reporterror="true"/>
    ```
    Send a PWM train. Chenge PWM sign when the position is reached

-   **applyforce**

    Apply wrench to the robot.


### 4.5.3. Yarp walking commands


-   **setvelocity**

    ```xml
        <command name="setVelocity" xvelocity="0.05" yvelocity="0.00001" repetitions="1" wait="0" reporterror="true"/>
    ```

    The attributes xvelocity and yvelocity rapesent the velocity in that direction.



-   **checkposition**
    ```xml
        <command name="checkPosition" xminposition="0.14" yminposition="0.13" zminposition="0.15" repetitions="1" wait="0" reporterror="true"/>
    ```
    Check if joints are present.


-   **preparestraightwalking**

    ```xml
        <command name="prepareStraightWalking" repetitions="1" wait="0" reporterror="true"/>
    ```

    No params needed

-   **startwalking**

    ```xml
        <command name="startWalking" repetitions="1" wait="0" reporterror="true"/>
    ```

    No params needed

-   **resetwalking**

    ```xml
        <command name="resetWalking" repetitions="1" wait="0" reporterror="true"/>
    ```

    No params needed




    If the robot position is less than xminposition,yminposition,zminposition the check fails. If the position is 0 it doesn't check on that axis.
    For now only the abs value of the position is checked.

## Test writing with scriptbuilder
Script builder is an UI for test and test list writing.

![alt text](img/img002.png "Scriptbuilder.")

TODO

## 4.6. Parametric test

It is possible to execute the same test many times changing one ore more parameters every execution. 

@subsection table-blocktest Table

We use a file, to specify the parameters value and parameters changing rules.

```xml
<settings robotname="icubSim" realrobot="false"  onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce" simclock="true" neverexecutecommands="" logseverity="debug" loggingtime="0.008" tablename="test/tables/main.tab"/> 
```

In the settings section you can find the **tablename parameter** (**main.tab**) that is the file in which
all the tables are written.
Inside of the file  you can find the tables that contain the parameters:

```
(this is the first table)
[
kppos       (table name)
normal      (table type)
1           (increment by time)

40 
50
60
70
80
90
]

(this is the second table)
[
xvelocity   (table name)
increment   (table type)
1           (increment by time)

1           (increment unit)
1           (min)
10          (max)
]
```

Here we have two parameters, kppos and kp in two tables.
It is possibile to specify how the parameters will change during the execution. There are two ways to do so, throught the **table type** parameter.

-   normal:
    in this case the parameter will have the value in the order the values are specified.
    In the above example the kppos parameter will have the value, in order, from
    the first to the last execution 40 50 60 70 80 90

-   increment
    in this case the parameter value will start from the min value to the max value using the
    given increment.

## 4.7. Using the parameter

In the test you can specify the table name you want to use, the paameter name is surround by 
$:

```xml
 <command name="setVelocity" xvelocity="$xvelocity$" yvelocity="0.5" repetitions="1" wait="0" reporterror="true"/>
```

In this case the xvelocity parameter will use the table xvelocity.

The test should be executed at least 10 times so:
```xml
    <test file="test//411.xml" repetitions="10" name="xxx" note="xxx" code="0411" version="1" loggingtype="" loggingwrappername="" loggingpart=""/>
```

## 4.8. Examples
Not regression tests and example are present in folder test.

# 5. Model settings

TODO

# 6. Test execution

To execute the test you should compile the test environment:

-   *cd ./build/blocktest*
-   *make rebuild_cache*
-   *make*
-   *make install*

from ./build/blocktest
and execute:

-   *./blocktest*

If you need to execute the test on the real robot use the appropriate settings:

```xml
    <settings robotname="icubSim" realrobot="true" onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce" simclock="false" neverexecutecommands=""/>
```

No prerequisites are necessary in this case.

# 7. Logging

Various logging file are present in the system.

## 7.1. Application logging

Application logging is log/log.log You can read it by:\
*tail -f log.log*

It is possibile to set the logs line to be shown using the attribute  logseverity="debug" inside of the

```xml
    <settings robotname="icubSim" realrobot="false"  onlysimcommands="yarpcheckrobotisvertical checkRobot yarpreset applyForce" simclock="false" positionfor="" neverexecutecommands="" logseverity="debug"/>
```

The awailable levels are:

    critical = 7
    exception = 6
    error = 5
    warning = 4
    info = 3
    trace = 2
    debug = 1
    none = 0

There are 10 logs used in a circular way.

## 7.2. Sensors logging

If sensor logging is setted (See section [test list](#testlist)) throught
**position** parameter in loggingtype, the position log file has the format: \<test number>-\<joint name>-\<repetition number>.
If logging is setted throught **com** parameter the position log file has the format: \<test number>-CoM-\<repetition number>.

```xml
    <test file="test//000.xml" repetitions="1" name="Position to 0" note="none"  code="0000" version="1" loggingtype="position com"  loggingwrappername="/right_leg" loggingpart=" L_AK_R R_AK_R"/>
```  

## 7.3. Plot logging

The command **nop** write on the file log/plot.log.

## 7.4. Report

At the end of the test a report summary is been written:

(2019-05-21 11:43:24.490)(Info*****)====================================\
(2019-05-21 11:43:24.490)(Info*****)===============Report===============\
(2019-05-21 11:43:24.490)(Info*****)====================================\
(2019-05-21 11:43:24.490)(Info*****)Test code:0100 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0101 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0102 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0103 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0104 error number:0\
(2019-05-21 11:43:24.490)(Info*****)Test code:0105 error number:0\
(2019-05-21 11:43:24.490)(Error****)Test code:0106 error number:2\
(2019-05-21 11:43:24.490)(Info*****)====================================\
(2019-05-21 11:43:24.490)(Info*****)====================================\
(2019-05-21 11:43:24.490)(Info*****)====================================\

@section future-blocktest Future

## 7.5. Timeout

Timeout for tests, after the timeout the test is failed.

# 8. Specific library writing

TODO

# 9. Bugs

None to be signaled

# 10. Authors
* Luca Tricerri ([*personal page*](http://www.iit.it/en/people/Luca-tricerri.html))


