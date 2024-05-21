# Dementia Guardian
clone into your device

npm install 

npm install leaflet 

npm install react-leaflet 

npm install axios



Dementia Guardian: An IoT-Based Monitoring and Assistance Device for Dementia Patients



Objective
The objective of the Dementia Guardian project is to develop an IoT device that ensures the safety and well-being of dementia patients. The device provides continuous monitoring, real-time location updates, and assistance in case of disorientation. It aims to mitigate the risks associated with wandering and getting lost, common issues faced by dementia patients.

Hardware Selection and Design
Sensors, Actuators, Controller, and Communications Units
 ![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/33dac555-a39e-4703-ab47-d0a83c328d37)

•	ESP32 Microcontroller: Chosen for its integrated Wi-Fi and  Blue- tooth capabilities, low power consumption, and versatility in IoT ap- plications.
•	GPS Sensor: Utilized to obtain real-time location data of the patient. This sensor activates when the patient presses the button on the device, providing accurate location information to the caregiver.
•	Button: A simple input device for the patient to alert the caregiver and request assistance. Pressing the button sends the patient’s current location to the caregiver.
 




•	Display: A screen to show messages to the patient, such as ”Press the button if you forget” and navigation routes to guide them home.

Parameters to be Monitored	1)GPS location (home and current)
    2)Button State
Sensors Used	    1)Neo GPS Module
    2)Push Button
    3)TFT LCD Display
Cloud Platform	    Render Cloud Service
Web socket Communication	1)	HTTP REST 
2)	FastAPI 
Database	    PostgreSQL
Backend /Interface	    Python



Software Design
Flowchart / Data Flow Diagram (DFD) / Pseudo Code
 ![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/e146cb88-9ed0-49aa-83cc-492449be2b47)


Flowchart
1.	Start
2.	Initialize ESP32, GPS, Button, and Display
3.	Patient presses button
•	a. Capture location using GPS
•	b. Send location to caregiver via web application
•	c. Display message to patient
4.	Monitor perimeter
•	a. If patient crosses perimeter, alert caregiver
5.	Record interactions (button presses, perimeter breaches)
6.	End Pseudo Code
 

I n i t i a l i z e   ESP32 ,  GPS,   Button ,	Display

while True :
i f   Button  i s	pre s s e d :
l o c a t i o n =  GPS. g e t  l o c a t i o n ()
s e n d l o c a t i o n   t o   c a r e g i v e r ( l o c a t i o n )
Display . show message ( ” Press - the - button - i f - you - f o r g e t ” )

i f	p a t i e n t	c r o s s e s	perimeter :
s e n d a l e r t   t o   c a r e g i v e r ( l o c a t i o n )

![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/5051fbe1-8895-4cc0-a9fa-4b862dcf4e7d)



Architecture diagram:


 
![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/8e4d8fdb-e6e3-4fa2-a3c0-1b8dd01ace5e)


 




r e c o r d i n t e r a c t i o n ( ” perimeter - breach ” ) r e c o r d i n t e r a c t i o n ( ” button - p re s s ” )


DFD Diagram:
 



![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/97510300-9eee-446b-9ff5-8c2fcaad2f05)





IoT System Design (Level 5 / 6)
![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/71bc16fc-181d-476b-a0e7-4daf3231515e)

 
Level 5/6 Design
The IoT system design for the Dementia Guardian device includes:
1.	End Device (ESP32, GPS, Button, Display): Captures and pro- cesses patient data, sends alerts, and displays messages.
2.	Edge Computing: Processes data locally on the ESP32, reducing latency and bandwidth usage.
3.	Cloud Integration: Uses cloud services for data storage, processing, and analysis. The web application for caregivers is hosted on the cloud, providing real-time access to patient data.

Integration of End, Edge, and Cloud
•	End Device: Collects data (location, button presses) and sends it to the Edge.
•	Edge Computing: Processes data locally on the ESP32 for immediate actions (alerts, display messages).
•	Cloud: Stores data for long-term analysis and provides a web appli- cation for caregivers to monitor patients in real-time.

Data Accumulation and Processing
 ![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/efd2d675-6677-4a7c-8673-5e216f91646d)

•	Data Accumulation: All interactions (button presses, perimeter breaches) are recorded and sent to the cloud.
•	Processing: Data is processed to generate insights such as frequency of disorientation (button presses) and perimeter breaches.
 



IoT Analytics

 ![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/1a8bae1c-3c07-4407-951c-d7308a30ebb9)

 ![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/8ef8d091-43ea-4911-b10f-cc40f21dbd32)

 ![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/4137f65f-7a36-4b99-a4b7-2331ffcaba37)

 ![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/046626d8-38ad-4397-9b59-8f6b09266bb3)
![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/f685db38-17b8-4a52-8254-79307e788376)

 
 

•	Alert Frequency over time : Tracks alerts over the whole period of time
•	Alert Source distribution : pie chart of distribution between button alerts and perimeter breach.
•	Time between alerts: Analyzes the alert frequency over an internal
•	Daily alert patterns : Alert pattern over a daily basis
•	Hourly alert trends : alerts in an hourly trends.


![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/e0c1a63a-4f1c-4bb9-9c1e-8fff2d1f4115)

 
Sample Test Cases and Reports
 ![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/071d0090-96da-4e09-a2b1-20e509a3245c)

Test Case 1: Button Press Functionality
•	Objective: Verify that pressing the button sends the correct location to the caregiver.
•	Steps:
1.	Patient presses the button.
2.	Check if the caregiver receives the location update.
3.	Confirm that the message is displayed on the device.
•	Expected Result: Caregiver receives accurate location, and message is displayed.
![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/3b97bd1e-36e1-476e-b6d9-661d73ac2a8b)

Test Case 2: Perimeter Breach Alert
•	Objective: Ensure that crossing the perimeter triggers an alert to the caregiver.
•	Steps:
1.	Patient crosses the predefined safe perimeter.
2.	Check if the caregiver receives an alert with the location.
•	Expected Result: Caregiver receives alert with accurate location.
	 
Test Case 3: Data Recording
•	Objective: Validate that all interactions are recorded accurately.
•	Steps:
1.	Press the button and cross the perimeter multiple times.
2.	Check the recorded data in the cloud.
•	Expected Result: All interactions are logged correctly.

Test Case 4: Medicine Alerts
 
•	Objective: Caregiver set the period of interval between timely alerts.
•	Steps:
1.	Set the interval and medicine names.
2.	Check for alerts in hardware display when time is up.
•	Expected Result: Repeated periodic alerts or medicine










![image](https://github.com/Barathj121/dementiaguardian/assets/110909380/fbc8447f-c9d6-48d0-9403-932618c24079)



Test Case 5: Patient Current Location
 

•	Objective: To check patients current location by the caregiver at any point of time.
•	Steps:
1.	Patient device to be on so it can send location periodically.
2.	Caregiver to check in his webapp for the current location 
•	Expected Result: Accurate realtime current location of patient.


Outcomes
•	Real-time Monitoring: Continuous tracking of patient location en- sures their safety.
•	Immediate Assistance: Patients can easily request help by pressing a button.
•	Caregiver Alerts: Instant notifications for perimeter breaches and disorientation incidents.
•	Data Insights: Analysis of interaction data aids in understanding patient behavior and improving care.
•	Medication Reminders: Adding functionality to remind patients of their medication schedules.
•	Monitor patient Current Location: Caregiver can constantly check the patient’s current location


Conclusion and Future Scope
Conclusion
The Dementia Guardian project successfully developed an IoT device that enhances the safety and well-being of dementia patients through real-time monitoring, location updates, and caregiver alerts. The system’s ability to record and analyze patient interactions provides valuable insights for improv- ing patient care.

Future Scope
•	Navigation Assistance: Implementing navigation routes on the dis- play to guide patients home.
 

•	Task Reminders: Integrating reminders for daily tasks and appoint- ments to further support patient independence.
