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
  gw.begin(nodeID);
  char ip[] = "10.1.3.1";
  char subnet[] = "255.255.0.0";
  gw.setIP(ip,subnet);
  uint32_t failCounter = 0;
  
  while(1){
    gw.update();
    if( network.available() ){
      RF24NetworkHeader header;
      size_t size = network.peek(header);
      uint8_t buf[size];
      network.read(header,&buf,size);
      printf("Received Network Message, type: %d id %d from %d\n",header.type,header.id,mesh.getNodeID(header.from_node));
      RF24NetworkFrame frame = RF24NetworkFrame(header,buf,size); 
      gw.sendUDP(mesh.getNodeID(header.from_node),frame);
    }
    delay(2);
    
    if(millis()-mesh_timer > 30000 && mesh.getNodeID()){ 
      mesh_timer = millis();
      if( ! mesh.checkConnection() ){
        mesh.renewAddress();
      }
    }
    if(radio.failureDetected > 0 || radio.getDataRate() != RF24_1MBPS){
      radio.failureDetected = 0;
      std::ofstream myFile;
      myFile.open ("failLog.txt");
      if (myFile.is_open()){
        myFile << ++failCounter << "\n";
        myFile.close();
      }
      delay(500);
      mesh.begin();
    }
  }
  return 0;
}
