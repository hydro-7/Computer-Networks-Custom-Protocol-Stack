# Computer-Networks-Custom-Protocol-Stack

# Introduction 

This is a custom protocol stack, which simulates the working of the Application Transport and Network layers found in computer networks. It is implemented using C++ and the program outputs the various resulting payloads found at each of the layers. An inspiration is taken from the existing protocol stack as found in the forouzan book, with slight modifications which should hopefully lead to higher security for important flagged files.

# Layered Approach to Computer Networks 

Although there are no physically different layers in computer networks, for the ease of our understanding, we often divide networks into the following layers : Application, Transport, Network, Data-Link and Physical. And as found in forouzan, I use a top down approach to look at these layers. Of these 5 layers, this repository contains an implementation of the top 3 layers with a slight modification to each of those layers. 
![image](https://github.com/user-attachments/assets/d1489b33-e731-48b2-a994-a3f6f4d0e95b)

# Application Layer 

The first layer, which acts as an interface between the user and rest of the layers is the application layer. This layer is where all the application we use are developed and maintained. Some common application layer protocols are : FTP (File Transfer Protocol), HTTP (Hypertext Transfer Protocol) and DNS (Domain Name System). 

Of this, I implemented a FTP based protocol for my application layer. This basically takes a file input from the user as well as a user input which would classify the file as important or unimportant. These details will be relevant in the lower layers. Further, on getting the file, the contents of it are considered the input data for the application layer. Onto this data a reversible encryption is done such that the data remains secure.

# Transport Layer 

The layer right after the application layer, which uses the encrypted message from the application layer as a payload, onto which the transport header is attached leading to the formation of a user datagram / segment (Based on the protocol used). 

For this implementation, I used a custom version of UDP, where one bit is removed from the Total Length field and a new field for Important Flag is added. This flag is set true if the user input in the application layer marked the file important and false otherwise.
![image](https://github.com/user-attachments/assets/2278fc6e-c042-41cc-bd8a-b6bcfca6b13f)

If the important flag is marked to be true, the UDP based protocol starts behaving more like a TCP protocol, wherein it starts expecting an ACK for the packets it sends (as these packets are deemed important by the user). On the other hand, if the flag is false, then it proceeds normally, similar to how normal packets would follow in a UDP protocol.
Note that the Acknowledgement workflow used here is a Stop and Wait based system, wherein the system doesnt show any progress until unless an Ack for the packets sent is recieved. This can easily be altered to Go Back N or even Selective Repeat based on user requirement. 

# Network Layer 

The output user datagram from the transport layer is used as the payload for the network layer. Onto this payload the IP header is attached which leads to the creation of the network layer datagram. Further based on a threshold MSS, this datagram will be fragmented while routing.

For this implementation I used a generic IP header format where one of the possible flags would be the Important flag. Further, the same details could be stored in the options/padding section of the datagram. This again would indicate whether or not the given packet/fragment should be considered important while routing.

Now, for the routing protocol, I made use of two methods, one being minimum hops (routers covered in path) and another being maximum bandwidth. Based on the important flag, the fragments would follow the min hops path if important and the max bandwidth path otherwise. Min hops path would lead to a reduction in packet loss probability whereas the max bandwidth path would lead to the maximum througput. 



