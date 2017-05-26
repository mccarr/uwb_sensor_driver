#include "tcp_client.h" //

// ROS publisher required includes:
#include <ros/ros.h>
#include <uwb_sensor_driver/UWB_reading.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv)
{



	// Initialise ROS and set up a node
	ros::init(argc, argv, "UWB_testing");
	ros::NodeHandle nh;

	// Create publisher and tell it to publish on topic /uwb_output
	ros::Publisher pub=nh.advertise<uwb_sensor_driver::UWB_reading>("uwb_output",100);
	
	// The rate of which is publish messages at - 10 Hz
	ros::Rate rate(10);
	
	
   
	// Initialise all variables
    string host = "10.0.1.177"; // IP address of the UWB Node (chosen in the arduino)
	int port = 80; 				// The port at which to transfer over (set in arduino)
    int tempInt; 				// Used to store next value from the stream as an int

	int numRanges = 1, numIntensities = 2, numColours = 3;
	int numVals = numRanges + numIntensities + numColours;

	while(ros::ok()){

    vector<int> values; 		// The array of readings as a changeable length array of ints
	stringstream stream;		// To hold the string reply from the server

	// Create the client object
	tcp_client c;

	// Connect to the host
    c.conn(host , port); 
     
    //send some data
    c.send_data("GET / HTTP/1.1\r\n\r\n");
     
	// Convert reply string into int vector (tokenized with space only atm AFAIK)
	// Input to function c.receive() is the number of bytes to listen to. This may need to be adjusted
		// if we change the code to listen to a longer message
    stream<<c.receive(1024);
	int i=0;
    while(stream>>tempInt){		
      // push tempInt onto values vector
    	values.push_back(tempInt);
    }
	
	//Assign the relevant parts
	vector<int> ranges (values.begin(),values.begin()+numRanges);
	vector<int> intensities (values.begin()+numRanges,values.begin()+numRanges+numIntensities);
	vector<int> colours (values.begin()+numRanges+numIntensities,values.begin()+numRanges+numIntensities+numColours);

	/* // Print the vectors
	
	for(int i=0; i < ranges.size(); i++){
		cout<<ranges[i]<<" ~ ";
	} cout<<endl;
	for(int i=0; i < intensities.size(); i++){
		cout<<intensities[i]<<" ~ ";
	}cout<<endl;
	for(int i=0; i < colours.size(); i++){
		cout<<colours[i]<<" ~ ";
	}cout<<endl;								*/
   
	// Now we have the readings, we can publish them to a ROS msg

	uwb_sensor_driver::UWB_reading msg;
	
	// Message has a header (with stamp, frame_id and sth else) and 3 int arrays
	msg.header.stamp = ros::Time::now();
	msg.header.frame_id = "/world"; // Might need to change this frame at some point in time. 
	// Vector assignment (both are vectors)
	msg.ranges = ranges;
	msg.intensities = intensities;
	msg.colours = colours; 

	// Publish the message
	pub.publish(msg);
	// Close the socket
	c.closeSock();
	// Sleep for the appropriate time so that we get the frequency "rate" as specified above
	rate.sleep();
		

	}
    return 0;
}
