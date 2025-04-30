[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/WXeqVgks)

# final-project-skeleton

* Team Number:
* Team Name:
* Team Members:
* GitHub Repository URL:
* GitHub Pages Website URL: [for final submission]

## Final Project Proposal

### 1. Abstract

*In a few sentences, describe your final project.*

The goal of this final project is to detect and track bruxism (involuntary grinding of teeth). The device is worn as a headband when you're asleep, that uses EMG sensors to detect when your jaw muscles contract. It displays the results on an LCD screen that graphs how much time you spent grinding. There could also be a version with day time use, where it simply makes a sound whenever someone is grinding. This will alert them so they consciously stop grinding.

### 2. Motivation

Bruxism can lead to serious complications for a person's dental and oral health. It can cause jaw/tooth pain, wear your teeth down, and contribute to TMJ disorders. Current methods to diagnose nightly bruxism are cumbersome because it can involve having to get a sleep study. Simply wearing the device for a couple of nights offers a simpler solution for doctors to diagnose this issue.

### 3. System Block Diagram

*Show your high level design, as done in WS1 and WS2. What are the critical components in your system? How do they communicate (I2C?, interrupts, ADC, etc.)? What power regulation do you need?*

![Block Diagram](images/block_diagram.png)

### 4. Design Sketches

*What will your project look like? Do you have any critical design features? Will you need any special manufacturing techniques to achieve your vision, like power tools, laser cutting, or 3D printing?*

Design considerations:

1. Placement of sensors and electronics
2. Sensitivity of sensor that measures contraction
3. 

Manufacturing

1. There won't be any special manufacturing techniques other than 3D printing and/or laser cutting an enclosure for the electronics.

# Design Sketch

![Design Sketch](images/design_sketch.png)

### 5. Software Requirements Specification (SRS)

*Formulate key software requirements here. Think deeply on the design: What must your device do? How will you measure this during validation testing? Create 4 to 8 critical system requirements.*

*These must be testable! See the Final Project Manual Appendix for details. Refer to the table below; replace these examples with your own.*

**5.1 Definitions, Abbreviations**

Here, you will define any special terms, acronyms, or abbreviations you plan to use for hardware

EMG: Electromyography

LCD: Liquid crystal display

Temporalis muscle: Jaw muscle that can be felt contracting on one's temple

**5.2 Functionality**

| ID     | Description                                                                                                           |
| ------ | --------------------------------------------------------------------------------------------------------------------- |
| SRS-01 | The EMG sensor will detect changes in muscle activation of the temporalis muscle                                      |
| SRS-02 | The ADC will convert the analog signals from the EMG into digital values                                              |
| SRS-03 | The LCD display will show the intensity of grinding over time for the night (using the real time clock to track time) |
| SRS-04 | During the day time a buzzer sounds when the clenching is above a certain threshold                                   |
|        | LCD will comunicate over SPI                                                                                          |
|        | RTC will communicate over I2c                                                                                         |
|        | ESP32 will communicate to atmega over uart, and it will communicate with the blynk iot app                            |
|        |                                                                                                                       |

### 6. Hardware Requirements Specification (SRS)

*Formulate key hardware requirements here. Think deeply on the design: What must your device do? How will you measure this during validation testing? Create 4 to 8 critical system requirements.*

*These must be testable! See the Final Project Manual Appendix for details. Refer to the table below; replace these examples with your own.*

**6.1 Definitions, Abbreviations**

Here, you will define any special terms, acronyms, or abbreviations you plan to use for hardware

ADC (analog to digital conversion)

**6.2 Functionality**

| ID     | Description                                                                           |
| ------ | ------------------------------------------------------------------------------------- |
| HRS-01 | The EMG sensors should be substantially sensitive to detect small muscle contractions |
| HRS-02 | The ADC module should support sufficient resolution for the signal                    |
| HRS-03 | The headband should be somewhat comfortable                                           |
| HRS-04 | Have a real time clock that can track over long periods time                          |
|        | Power module with battery add-on and buck converter                                   |
|        | High pass RC filter                                                                   |
|        | ESP32 with logic shifter                                                              |

### 7. Bill of Materials (BOM)

*What major components do you need and why? Try to be as specific as possible. Your Hardware & Software Requirements Specifications should inform your component choices.*

*In addition to this written response, copy the Final Project BOM Google Sheet and fill it out with your critical components (think: processors, sensors, actuators). Include the link to your BOM in this section.*

https://docs.google.com/spreadsheets/d/1-zWV1aL9oH2EXkD6tylsm7H7TjFfnVomCbpaZuMzBcA/edit?usp=sharing

### 8. Final Demo Goals

*How will you demonstrate your device on demo day? Will it be strapped to a person, mounted on a bicycle, require outdoor space? Think of any physical, temporal, and other constraints that could affect your planning.*

For demo day we will test the device on one of our team members, and show the LCD updating in real time as well as the buzzer.

### 9. Sprint Planning

*You've got limited time to get this project done! How will you plan your sprint milestones? How will you distribute the work within your team? Review the schedule in the final project manual for exact dates.*

| Milestone  | Functionality Achieved                                       | Distribution of Work                            |
| ---------- | ------------------------------------------------------------ | ----------------------------------------------- |
| Sprint #1  | Reading data from muscle contraction                         | 1 team member sets up hardware, other 2 code it |
| Sprint #2  | LCD graph of clenching time, buzzer that alerts to clenching | all 3 team members code and debug               |
| MVP Demo   | Integration of previous sprints                              | 3 team members optimize embedded system         |
| Final Demo | All parts working along with sleeker mechanical design       | All members work on mechanical design and final |

**This is the end of the Project Proposal section. The remaining sections will be filled out based on the milestone schedule.**

## Sprint Review #1

### Last week's progress

Over this week our goal was to try to get the jaw clenching sensing down and interfaced with the atmega. We made significant progress with this by testing various sensors and we were successfully able to read values from the force sensitive resistor. We set up a voltage divider with the sensor and a set resistance value and converted the analog signal to a digital one. We aren't sure exactly if this will pan out to the final product because it's not very sensitive to the muscle movements. So we also tested various EMG sensors that we ordered/had available. They seem to be tracking the muscle contractions better (since thats what they're actually built for. We're able to read analog values on the scope, but they will need a lot more processing for the final product.

[https://drive.google.com/file/d/1IZ25nzlBHVpCWKgTvnpPDgVcIyl4WT8e/view?usp=drive_link](https://urldefense.com/v3/__https://drive.google.com/file/d/1IZ25nzlBHVpCWKgTvnpPDgVcIyl4WT8e/view?usp=drive_link__;!!IBzWLUs!UGNzIMPeejVOO6jmQ7SIQz7D42Xgi_ii_dkTf_qsw6nuW17fmtB2CERJciw6ATmgj1CIfD-GhR4P2WBRqG_7nw3z$)

### Current state of project

One sensor option working, (thought we were told it wasn't complex enough).

### Next week's plan

Divide and conquer. One team member works on the RTC and LCD screen, one team member figures out the signal processing, one team member figures out the app.

## Sprint Review #2

### Last week's progress

Over the past week our goal was to solidify the sensor readings and get the LCD screen plotting graphs. So far we have gotten the LCD screen to display a graph image and have successfully plotted dummy graph data. We got the EMG sensor to output an analog signal that can be succesfully converted to digital values. There's still some work left to do to process this data and then display it on the graph. We also got the real time clock to work over i2c. Another group member was working on the ESP32 app integration, but there were some debugging issues there.

https://drive.google.com/file/d/1-YQYbSzBp0TBx5U1PUz_vQ8te4gLAX2D/view?usp=sharing

https://drive.google.com/file/d/1UxyY6kfUbMNISJIG7tiHbU9iLg-6Rss-/view?usp=sharing

### Current state of project

We essentially just have to integrate all the parts of the project together, and get the app working.

### Next week's plan

Over this next week we will finalize each individual piece of the project and then put it together.

## MVP Demo

1. Show a system block diagram & explain the hardware implementation.

   <img src="blockDiagramMVP.png" alt="Block Diagram" width="400" height="300">
2. Explain your firmware implementation, including application logic and critical drivers you've written.

   Most of the LCD implementation was taken from Lab4, and we added the logic/code that generates a graph over time. For the real time clock, we had to create our own i2c library for it. After setting the necessary register bits in the atmega for i2c, we created a few functions that set the time and get the time of the clock. Time is stored in a few registers from 00 to 07. The registers are seconds, minutes, hours, day, month year. In order to set time, we have to write the address of the clock, then the address of the register that we want to write, then the value in BCD format. Getting time is a similar process, but in reverse.
3. Demo your device.

   https://drive.google.com/file/d/1qDcoZRG4p9KLhYcpPG2Q98YLUuNXgrrb/view?usp=sharing
4. Have you achieved some or all of your Software Requirements Specification (SRS)?

   We have achieved most of our software requirements but not all of them

   - We WERE able to get the EMG sensor to detect the temporalis muscle clenching. We first made sure we got analog values by hooking up to a scope, then proceeded to implement interfacing with the atmega
   - We WERE able to convert the analog sensor values to digital values with the atmega onboard ADC. After we got the scope to read the sensor values, we implemented communication with the atmega.
   - The LCD DOES show the intensity of clenching over time on a graph
   - We have not yet impemented the buzzer sound that occurs during the day when a clenching event occurs. At this point it will be an addon if we have time, as our ultimate goal is implementing the night diagnoses
   - The LCD DOES communicate over SPI
   - The RTC DOES communicate over I2C
   - We have had trouble getting the ESP32 to communicate with the atmega32, so have not accomplished this yet
5. Have you achieved some or all of your Hardware Requirements Specification (HRS)?

   - The EMG sensor WAS able to pick up the small muscle contractions of the jaw muscle. This is quantified by the range of voltage that the emg sensor would pick up. When there was no clenching, the scope read 0 V. When their was clenching there was about 3V.
   - The ADC module DOES have sufficient resolution to convert the analog values of the sensor to digital values. After hooking it up to the atmega, we were able to get the ADC conversion to scale from 0 to 1023, which was our goal.
   - We haven't gotten to the point of putting the whole product together with the headband (as all the electronics are still a jumble of wires), however currently when wearing the headband with just the sensor it's fairly comfortable.
   - The real time clock worked very well
   - We added a custom power module in order to run the device when not plugged in to the computer. It is simply a buck converter with a few external components surrouonding it (mostly capacitors and the battery mount thing). Now, when a 9V battery is connected, the whole system can be powered which is the ultimate goal for this product (since you don't want to sleep with something plugged into an outlet/computer).
   - In order to get the EMG sensor values to properly be read, we had to include an RC high pass filter that filtered out the noise. This is ultimately what made us able to read the values accurately.
   - The ESP32 and logic shifter were not included in our original HRS so this is a note that we added those componenets
6. Show off the remaining elements that will make your project whole: mechanical casework, supporting graphical user interface (GUI), web portal, etc.

   We begun the design of the mechanical casing in solidworks, but still need to make significant progess with that.
7. What is the riskiest part remaining of your project?
   The riskiest part will be transferring the completed circuit to the mechanical enclosure (where we will also try to switch out the wires to the flat ones that don't clutter up the circuit). This is risky because if we make any mistakes in doing this, we will have to debug the hardware for an issue, which will be stressful as we approach the final demo day deadline.

   1. How do you plan to de-risk this?
      We will make sure to transfer each subsystem one by one and very carefully, and then test the code for that subsystem to make sure that it works.
8. What questions or help do you need from the teaching team?

See ED discussion (the uart issues)

## Final Project Report

Don't forget to make the GitHub pages public website!
If you’ve never made a GitHub pages website before, you can follow this webpage (though, substitute your final project repository for the GitHub username one in the quickstart guide):  [https://docs.github.com/en/pages/quickstart](https://docs.github.com/en/pages/quickstart)

### 1. Video

https://drive.google.com/file/d/1p764nsIRFncH8L2IfexEU7o_ynaXjqQ5/view?usp=sharing

* The video must demonstrate your key functionality.
* The video must be 5 minutes or less.
* Ensure your video link is accessible to the teaching team. Unlisted YouTube videos or Google Drive uploads with SEAS account access work well.
* Points will be removed if the audio quality is poor - say, if you filmed your video in a noisy electrical engineering lab.

### 2. Images


<p float="left">
  <img src="images/front_view.png"   alt="Front View"  width="120px" />
  <img src="images/back_view.png"    alt="Back View"   width="120px" />
  <img src="images/left_view.png"    alt="Left View"   width="120px" />
  <img src="images/right_view.png"   alt="Right View"  width="120px" />
  <img src="images/top_view.png"     alt="Top View"    width="120px" />
  <img src="images/inside_view.png"  alt="Inside View" width="120px" />
</p>

*Include photos of your device from a few angles. If you have a casework, show both the exterior and interior (where the good EE bits are!).*

### 3. Results

*What were your results? Namely, what was the final solution/design to your problem?*

The final design was a laser cut enclosure with a cutout for and LCD screen and a spot to insert the sensor. The box has an on/off switch and the device would start graphing grinding data after the current time (from the real time clock) reached the set bedtime value. The demo video shows the graphing on a scale of 5 mins, but in real life the scale would be a normal amount of sleep (e.g. 8 hours). The app component of the project worked fairly well too. The esp32 connected the Blynk IoT app which sent bedtime and waketime data to it, which sent that data to the atmega. Once those were sent, the atmega would send sensor data to the esp32 (and then to the Blynk IoT interface) in order for the user to calibrate the location of the headband/sensor on their temple. Unfortunately, we had to run this project on a separate atmega, because the uart communcation didn't work from the esp32 to the atmega with the rest of the components and software (LCD, clock, and sensor)

#### 3.1 Software Requirements Specification (SRS) Results

*Based on your quantified system performance, comment on how you achieved or fell short of your expected requirements.*

*Did your requirements change? If so, why? Failing to meet a requirement is acceptable; understanding the reason why is critical!*

*Validate at least two requirements, showing how you tested and your proof of work (videos, images, logic analyzer/oscilloscope captures, etc.).*

| ID     | Description                                                                                                                                              | Validation Outcome                                                                                                                                                                                                                                                                                                                                                                       |
| ------ | -------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| SRS-01 | The EMG sensor will detect changes in muscle activation of the temporalis muscle                                                                         | Confirmed, the EMG sensor was able to precisely detect when the temporalis muscle moved                                                                                                                                                                                                                                                                                                  |
| SRS-02 | The ADC will convert the analog signals from the EMG into digital values                                                                                 | Confirmed, we were able to observe the ADC values from the EMG sensor match the magnitude of its analog signal                                                                                                                                                                                                                                                                           |
| SRS-03 | The LCD display will show the intensity of grinding over time for the night (using the real time clock to track time).<br />LCD will comunicate over SPI | Confirmed. The LCD screen graphs the values from the EMG sensor appropriately                                                                                                                                                                                                                                                                                                            |
| SRS-04 | During the day time a buzzer sounds when the clenching is above a certain threshold                                                                      | Failed, never had time to implement this feature, as we prioritized detection of night bruxism                                                                                                                                                                                                                                                                                           |
| SRS-05 | RTC will communicate over I2c to update the atmega on what the current time is                                                                           | Confirmed, we were able to code functions that allowed the RTC to communicate over I2C with the atmega                                                                                                                                                                                                                                                                                   |
| SRS-06 | ESP32 will communicate to atmega over uart to atmega, and it will communicate with the blynk iot app                                                     | Semi-confirmed, we were able to get the blynk app working and the esp32 communicating with AN atmega, but<br />it wouldn't work when integrated with the whole system. We believe this was because the atmega was running out <br />of memory/storage, because we changed nothing when testing with the other atmega except for removing<br />the other main components of the project. |

#### 3.2 Hardware Requirements Specification (HRS) Results

*Based on your quantified system performance, comment on how you achieved or fell short of your expected requirements.*

*Did your requirements change? If so, why? Failing to meet a requirement is acceptable; understanding the reason why is critical!*

*Validate at least two requirements, showing how you tested and your proof of work (videos, images, logic analyzer/oscilloscope captures, etc.).*

| ID     | Description                                                                           | Validation Outcome                                                                                                                                                                                                                                |
| ------ | ------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| HRS-01 | The EMG sensors should be substantially sensitive to detect small muscle contractions | Confirmed. The EMG sensor we purchased fit out application perfectly. It only picked up the small movements<br />ouf the temporalis muscle, without picking up the noise of someone tossing and turning in bed                                    |
| HRS-02 | The ADC module should support sufficient resolution for the signal                    | Confirmed, the onboard ADC was able to pick up the analog values from the sensor                                                                                                                                                                  |
| HRS-03 | The headband should be somewhat comfortable                                           | Semi-confirmed. The headband itself isn't bad, however the box attachment is somewhat<br />cumbersome to deal with. Ideally the product would be on a flex PCB built into the <br />headband.                                                    |
| HRS-04 | Have a real time clock that can track over long periods time                          | Confirmed, the RTC worked well                                                                                                                                                                                                                    |
| HRS-05 | Power module with battery add-on and buck converter                                   | Confirmed, though not in the demo video, we had the battery hooked up to the<br />entire system, and included a button on the side. The device would turn on when the button<br />was pressed and begin graphing (once the time reached bedtime). |
| HRS-06 | High pass RC filter                                                                   | We added this once we figured out that the EMG sensor analog values (which we red from the<br />scope in detkin) had far too much noise. This filtered out the low frequency noise<br />which enabled us to correctly interpret the sensor data.  |
| HRS-07 | ESP32 with logic shifter                                                              | Semiconfirmed - the ESP32 worked on its own, however it turned out we didnt<br />even need the logic shifter (it worked when direclty plugged into the atmega). Though again,<br />that was with a separate atmega.                               |

### 4. Conclusion

Reflect on your project. Some questions to address:

* What did you learn from it?
  The biggest thing we learned was implementing serial communication with bare-metal programming. In ESE 1110 we all had similar projects however most people used an arduino and arduino libraries for every external device. This time we actually had to program how the atmega would communicate with, for example, the clock over I2C. We had to get the atmega to communicate with the esp32 over uart, and with the LCD over SPI. This let us gain a much deeper appreciation for the underlying engineering that went in to design even simple electronic components like these (because we had to actually read the datasheets).
* What went well?
  The biggest challenge that we were actually able to overcome was reading sensor values from the EMG sensor. Going into this project we thought that this is what would give us the most diffiiculty because there are so many obstacles to overcome: what if the muscle movements are too small and the sensor isn't sensitive enough, what if the analog value it outputs isn't a high enough voltage, etc. But we were successfully able to read the EMG sensor data, convert it with the ADC, and process it in our program. The front-end with the graph works pretty well, and the physical packaging itself we are proud. While we weren't able to get the esp32 communicating with the front end, the calibration and bedtime setting as a standalone feature works fairly well.
* What accomplishments are you proud of?
  Again the physical enclosure that we made looks fairly sleek so we are proud of that. We are also very proud of the power management system, because we had the device turn on/off with the click of a button. Getting the RTC working was another major accomplishment we are proud of, because acutally correctly programming a serial communication protocol is something none of us have ever done. The blynk IoT interface we designed looks pretty sleek, and again the fact that callibration worked was also something we were proud of.
* Did you have to change your approach?
  Yes there were numerous times we had to change our approach. For example when we were testing the sensor we realized that the ADC values were too noisy, so we had to add an RC high-pass filter and envelope extraction in software. When we were trying to get the esp32 to communicate with the atmega over uart, we first tried to do it with a logic level shifter. It wasn't working so we just scrapped it and direclty connected the uart to the atmega (which worked for some reason).
* What could have been done differently?
  We should have started testing on the communication between the atmega and esp32 much earlier because that seemed to be the biggest bottleneck and failure of our project. Also adding and SD card to the atmega because we suspect ours literally just ran out of memory.
* Did you encounter obstacles that you didn’t anticipate?
  There were a lot of issues that were very frustrating because they were seemingly just out of our control because something was wrong with the hardware. For example when programming the LCD screen at some point it literally just white screened and would not stop white screening no matter how many times we unplugged it, reproggamed it, or switched to a different LCD. The issue for some reason was resolved when I created a new project and slowly added back the code from my old project. Then it never came up again. Another issue was when we were attemping to program the esp32 it would sometimes just fail to upload from anyone's computer because it would stop being recognized by the serial port halfway through uploading. We tried everything to debug the issue but the only thing that worked was switching the esp32 to another one. Until, that one started to have the same issue. So we had keep switching to new esp32s whenever they stopped being able to upload. We then stumbled upon one that worked without any issues, but either way this caused us a lot of pain. Finally, we had a lot of issues getting the esp32 to communicate with the atmega. We got it communicating with AN atmega, but it wouldn't work when integrated with the whole system. We believe this was because the atmega was running out of memory/storage, because we changed nothing when testing with the other atmega except for removing the other main components of the project. This obstacle was never resolved
* What could be a next step for this project?
  Next step would be to actually get the esp32 communication with the atmega to work. Once the full embedded system prototype is working, we would transition to designing the circuit on a flex pcb, and integrating all the components into the headband so it's comfortable. Then we would partner with myTMJrelief to sell the device.

## References

Fill in your references here as you work on your final project. Describe any libraries used here.

We used the LCD SPI library from lab4 and the uart library from the previous labs as well.
