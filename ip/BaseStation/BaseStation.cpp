#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>  
#include <RF24Gateway/RF24Gateway.h>

RF24 radio(17,0);
RF24Network network(radio);
RF24Mesh mesh(radio,network);
RF24Gateway gw(radio,network,mesh);

uint32_t mesh_timer = 0;

int main(int argc, char** argv) {

  uint8_t nodeID=0;
  gw.begin(nodeID); // Start the gateway using RF24Mesh, with nodeID 0 (Master)
  char ip[] = "10.1.3.1";
  char subnet[] = "255.255.0.0";
  gw.setIP(ip,subnet); // configure the IP and subnet mask
  uint32_t failCounter = 0;
  
  while(1){
    gw.update(); // keep the network and mesh layers active. needs to be called on a regular basis. 
    if( network.available() ){
      RF24NetworkHeader header; // Headers are addressed to the appropriate node. 
      size_t size = network.peek(header); // Read the next available header
      uint8_t buf[size];
      network.read(header,&buf,size); // Read a message
      printf("Received Network Message, type: %d id %d from %d\n",header.type,header.id,mesh.getNodeID(header.from_node));
      RF24NetworkFrame frame = RF24NetworkFrame(header,buf,size); // The structure of frame, consist of : header, size, and message itself. 
      gw.sendUDP(mesh.getNodeID(header.from_node),frame); // sendUDP
    }
    delay(2);
    
    if(millis()-mesh_timer > 30000 && mesh.getNodeID()){ // if 30 sec has passed and NodeID exist
      mesh_timer = millis(); // update the timer as the current time. 
      if( ! mesh.checkConnection() ){ // if connectivity fails, address renewal should be invoked.
        mesh.renewAddress(); // reconnect to the mesh and renew the current RF24Network addr. 
      }
    }
    // check for failures and deviations from the default configuration (1MBPS data rate)
    if(radio.failureDetected > 0 || radio.getDataRate() != RF24_1MBPS){ 
      radio.failureDetected = 0;
      std::ofstream myFile;
      myFile.open ("failLog.txt");
      if (myFile.is_open()){ // failure count logged to failLog.txt
        myFile << ++failCounter << "\n";
        myFile.close();
      }
      delay(500);
      mesh.begin(); // The mesh is restarted on failure. Purpose: to make the radios hot-swappable. 
    }
  }
  return 0;
}
