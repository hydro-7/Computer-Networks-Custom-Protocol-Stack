#include<bits/stdc++.h>
using namespace std;

struct Datagram {
    string vr;
    string hl;
    string st;
    string tl;
    string id;
    string f;
    string offset;
    string ttl;
    string prt;
    string hcs;
    string sipa;
    string dipa;
    string opt;
    string payload;
};

struct Segment{
    string spa;
    string dpa;
    string tl;
    string cs;
    string imp;
    string message;
};

// ------------------------------------------------ GENERAL FUNCTIONS ---------------------------------------------------------- //

string get_checksum(string message)
{
    int xorvalue[16];
    for(int i = 0; i < 16; i++) xorvalue[i] = (int)message[i];

    for(int i = 16; i < message.size(); i++) {
        xorvalue[i % 16] ^= (int)message[i];
    }

    string final_xor = "";
    for(int i = 0; i < 16 && final_xor.size() < 16; i++) final_xor += to_string(xorvalue[i]);

    return final_xor;
}

// ----------------------------------------------- APPLICATION FUNCTIONS -------------------------------------------------------//

string message_encrypt(string message) 
{ 
    string encrypted = "";
    for(char x : message) {
        int dif = (x - 'a');
        encrypted += (x - 7 - (2 * dif));
    }
    return encrypted;
}

// ----------------------------------------------- TRANSPORT FUNCTIONS -------------------------------------------------------//

void Ack_Workflow() 
{
    string input;
    cout<<"\033[1mAck Recieved for Important Packet ? (Y/N) : \033[0m";
    cin>>input;

    if(input == "Y") {
        cout<<"\n\033[1mPacket Recieved by the Reciever !\033[0m\n";
        cout<<"\n\n";
        for(int i = 0; i < 80; i++) cout<<"=";
        return;
    }
    else {
        cout<<"\n\033[1mTime Out !\033[0m\n\033[1mResending Packet & Restarting Timer !\033[0m\n";
        Ack_Workflow();
        return;
    }
}


// ----------------------------------------------- NETWORK FUNCTIONS -------------------------------------------------------//

void print_router_graph() {
    cout<<"\n\n\n\033[1m ROUTER DIAGRAM \033[0m\nThe nodes A, B, C, D & E represent the routers in the network whereas the weights on the edges represent the respective bandwidths of the paths.\n\n";
    cout<<R"(
              :--- 2 ---- B ---- 2 ---- C ---- 2 ---:
              |                                     |
        A ----|                                     |---- D
              |                                     |
              :--------- 1 ----- E ----- 1 ---------:
    )";
    cout<<"\n\n"; 

}

string min_hops(int n, vector<pair<int, int>> adj[], int source, int destination) {
    vector<int> vis(n, 0);

    queue< pair< pair<int, int>, string> > q; // node, hops, path
    q.push({{source, 0}, to_string(source)});
    vis[source] = 1;

    while(!q.empty()) {
        int curr_node = q.front().first.first, curr_hops = q.front().first.second;
        string curr_path = q.front().second;
        q.pop();

        if(curr_node == destination) return curr_path;

        for(auto &next : adj[curr_node]) {
            int next_node = next.first, cost = next.second;

            if(!vis[next_node]) {
                vis[next_node] = 1;
                q.push({{next_node, curr_hops + 1}, curr_path + to_string(next_node)});
            }
        }
    }

    return "";
}

int max_weight = 0;
vector<int> best_path;

void dfs(int node, int y, vector<pair<int, int>> adj[], vector<bool>& visited, int current_weight, vector<int>& path) {
    path.push_back(node);         // Add current node to path

    if (node == y) {              // If destination node is reached
        if (current_weight > max_weight) {
            max_weight = current_weight;
            best_path = path;     // Save the current path as the best path
        }
        path.pop_back();          // Backtrack
        return;
    }

    visited[node] = true;

    for (auto &neighbor : adj[node]) {
        int next_node = neighbor.first;
        int weight = neighbor.second;

        if (!visited[next_node]) {
            dfs(next_node, y, adj, visited, current_weight + weight, path);
        }
    }

    visited[node] = false;        // Backtrack
    path.pop_back();
}

int max_bandwidth(int n, vector<pair<int, int>> adj[], int source, int destination) {
    vector<bool> visited(n, false);
    vector<int> path;
    max_weight = INT_MIN;

    dfs(source, destination, adj, visited, 0, path);

    return (max_weight == INT_MIN) ? -1 : max_weight;
}

// ------------------------------------------------ PACKET PRINTING --------------------------------------------------------- // 

void print_encrypted_message(string s) {
    int n = s.size(); cout<<"\n\n";
    for(int i = 0; i < 80; i++) cout<<"=";
    cout<<"\n\n\n";
    cout<<"\033[1mAPPLICATION LAYER OUTPUT : \033[0m\n\n";
    for(int i = 0; i < 34 + n; i++) cout<<"-";
    cout<<"\n|     Encrypted Message  :  "<<s<<"     |\n";
    for(int i = 0; i < 34 + n; i++) cout<<"-";
    cout<<"\n\n\n";
    for(int i = 0; i < 40; i++) cout<<"=";
}

void print_transport_payload(Segment s) {
    for(int i = 0; i < 40; i++) cout<<"=";
    cout<<"\n\n\n";
    cout<<"\033[1mTRANSPORT LAYER OUTPUT : \033[0m\n\n";
    for(int i = 0; i < 44; i++) cout<<"-";
    cout<<"\n|  "<<s.spa<<"  ||  "<<s.dpa<<"  |\n";
    for(int i = 0; i < 44; i++) cout<<"-";
    cout<<"\n| "<<s.imp<<" || "<<s.tl<<" || "<<s.cs<<" |\n";
    for(int i = 0; i < 44; i++) cout<<"-";
    cout<<"\n|             "<<s.message<<"             |\n";
    for(int i = 0; i < 44; i++) cout<<"-";
    
}

void print_network_payload(vector<Datagram> v) {
    cout<<"\n\n\n";
    cout<<"\033[1mNETWORK LAYER OUTPUT : \033[0m\n\n";
    int sz = v.size();

    for(int i = 0; i < sz; i++) {
        Datagram d = v[i];

        for(int ii = 0; ii < 48; ii++) cout<<"-";
        cout<<"\n| "<<d.vr<<" || "<<d.hl<<" || "<<d.st<<" || "<<d.tl<<" |\n"; // 48
        cout<<"\n| "<<d.id<<"  ||  "<<d.f<<"  ||  "<<d.offset<<" |\n"; // 48
        cout<<"\n| "<<d.ttl<<"  ||  "<<d.prt<<"  ||  "<<d.hcs<<" |\n"; // 48
        cout<<"\n|       "<<d.sipa<<"       |\n";
        cout<<"\n|       "<<d.dipa<<"       |\n";  
        cout<<"\n|                  "<<d.payload<<"                  |\n";
    }
    for(int ii = 0; ii < 48; ii++) cout<<"-";
}

// --------------------------------------------------------------------------------------------------------------------------- //


int main()
{
    // ---------------------------------------------- APPLICATION LAYER : FTP -------------------------------------------------------------- // 

    // We use encryption instead of hashing, because you cant reverse a hash for the decoder.
    cout << R"(
    ------------------------------------------------------------------------------------------------------
        _____ _             _   _                                    _ _           _   _                _ 
       / ____| |           | | (_)                 /\               | (_)         | | (_)              | |
      | (___ | |_ __ _ _ __| |_ _ _ __   __ _     /  \   _ __  _ __ | |_  ___ __ _| |_ _  ___  _ __    | |
       \___ \| __/ _` | '__| __| | '_ \ / _` |   / /\ \ | '_ \| '_ \| | |/ __/ _` | __| |/ _ \| '_ \   | |
       ____) | || (_| | |  | |_| | | | | (_| |  / ____ \| |_) | |_) | | | (_| (_| | |_| | (_) | | | |  |_|
      |_____/ \__\__,_|_|   \__|_|_| |_|\__, | /_/    \_\ .__/| .__/|_|_|\___\__,_|\__|_|\___/|_| |_|  (_)
                                         __/ |          | |   | |                                        
                                        |___/           |_|   |_|                                            
    ------------------------------------------------------------------------------------------------------ 
                                        )" << "\n\n\n\n\033[1mPlease input desired file name : \033[0m";

    string filename; cin>>filename;
    string message; 

    ifstream file(filename);
    getline(file, message);
    file.close();
    
    string is_important; cout<<"\033[1mIs the file very important ? (Y/N) : \033[0m"; cin>>is_important;

    string encrypted_message = message_encrypt(message); 
    print_encrypted_message(encrypted_message);

    // ----------------------------------------------- TRANSPORT LAYER : UDP ---------------------------------------------------------------- // 

    // string source_port_address = "1234567890123456", destination_port_address = "1234567890123456"; // size = 16 bits 
    string source_port_address = "sourcportaddress", destination_port_address = "destiportaddress"; // size = 16 bits 

    int transport_tl = 64 + encrypted_message.size(); // total length 64 bits + message length 
    string transport_total_len = to_string(transport_tl); // 15 bits, one bit moved to flag
    while(transport_total_len.size() < 15) transport_total_len.insert(transport_total_len.begin(), '0');
    
    string important_flag; // either 'T' or 'F'
    if(is_important == "Y") important_flag = "T"; else important_flag = "F";

    while(encrypted_message.size() % 16 != 0) encrypted_message += "0"; // padding the data : the final message should be a multiple of 16 bits
    
    string checksum = get_checksum(encrypted_message + source_port_address + destination_port_address + transport_total_len + important_flag);
    while(checksum.size() < 16) checksum.insert(checksum.begin(), '0');

    Segment segment = {source_port_address, destination_port_address, transport_total_len, checksum, important_flag, encrypted_message};
    string transport_payload = segment.spa + segment.dpa + segment.tl + segment.cs + segment.imp + segment.message;


    print_transport_payload(segment);
    if(important_flag == "T") {
        
        cout<<"\n\n\n\033[1mExpecting ACK for the important packet ! Timer started !\033[0m\n\n";
        // ack workflow
        Ack_Workflow();
    }


    // ------------------------------------------------ NETWORK LAYER : IPv4 ------------------------------------------------------------------ // 

    string source_ip_address = "12345678901234567890123456789012", destination_ip_address = "12345678901234567890123456789012"; // size = 32 bits 

    string ver = "ipv4"; // v4 or v4
    string service_type = "00000000"; // not using icmp / igmp so 0
    string time_to_live = "00000200"; // has to be less than 255
    string protocol = "UDP00UDP"; 
    
    string network_flag;
    string options;
    if(segment.imp == "T") {
        options = "IMPORTANT_MESSAGE"; // variable size
        network_flag = "IMP";
    } else {
        options = "REGULAR_MESSAGE";
        network_flag = "REG";
    }
    while(options.size() % 32 != 0) options.insert(options.begin(), '0');

    int hl = 160 + options.size(); // header + options
    string hlen = to_string(hl);
    hlen.insert(hlen.begin(), '0');

    int network_tl = hl + transport_payload.size();
    string network_total_len = to_string(network_tl); 
    while(network_total_len.size() < 16) network_total_len.insert(network_total_len.begin(), '0');

    // # Fragmentation of Segment : 
    // Basically, for a particular message, we consider a constant identification 'i' and varying fragmentation_offset values for the different fragments which the original
    // segment has been split into. to do this we consider a vector<string> where each segment (string) within this vector theoretically has a unique header from the network layer.

    int frag_threshold = 10, frag_count = 0;

    vector<Datagram> fragmented_datagrams;
    string identification = ""; // as we are simulating only one packet, the identification of this packet is 0
    for(int i = 0; i < 16; i++) identification += "0";

    if(transport_payload.size() > frag_threshold) {
        int frags;
        if(transport_payload.size() % frag_threshold == 0) frags = transport_payload.size()/frag_threshold;
        else frags = transport_payload.size()/frag_threshold + 1;

        vector<string> payloads;
        string temp = "";
        for(int i = 1; i <= transport_payload.size(); i++) {
            if(i % frag_threshold == 0) {
                temp += transport_payload[i - 1];
                payloads.push_back(temp);
                temp.clear();
            } 
            else {
                temp += transport_payload[i - 1];
            }
        }
        if(temp.size() != 0) payloads.push_back(temp);

        for(int i = 0; i < frags; i++) {
            string offset_value = to_string(frag_count);
            frag_count++;
            while(offset_value.size() < 13) offset_value.insert(offset_value.begin(), '0');   
            string network_cs = get_checksum(ver + hlen + service_type + network_total_len + identification + offset_value + time_to_live + protocol + source_ip_address + destination_ip_address + options);

            Datagram datagram = {ver, hlen, service_type, network_total_len, identification, network_flag, offset_value, time_to_live, protocol, network_cs, source_ip_address, destination_ip_address, options, payloads[i]};
            fragmented_datagrams.push_back(datagram);
        }
    }   
    else {
        string offset_value = "";
        for(int i = 0; i < 13; i++) offset_value += "0";
        string network_cs = get_checksum(ver + hlen + service_type + network_total_len + identification + offset_value + time_to_live + protocol + source_ip_address + destination_ip_address + options);

        Datagram datagram = {ver, hlen, service_type, network_total_len, identification, network_flag, offset_value, time_to_live, protocol, network_cs, source_ip_address, destination_ip_address, options, transport_payload};
        fragmented_datagrams.push_back(datagram);
    }


    print_network_payload(fragmented_datagrams);

    // ---------------------------------------------------------- ROUTING PROTOCOLS ----------------------------------------------------------------- //

    print_router_graph();

    vector<pair<int, int>> adj[5];
    adj[0].push_back({1, 2});
    adj[0].push_back({4, 1});
    adj[1].push_back({2, 2});
    adj[2].push_back({3, 2});
    adj[4].push_back({3, 1});

    // custom routing protocol : if the packet is regular, it follows the path with more bandwidth, regardless of how many routers fall in the way
    // if the packet is flagged as important, we transport it through a path with the least number of routers

    cout<<"The path picked by the packet is as follows : \n";
    if(network_flag == "IMP") {
        string path = min_hops(5, adj, 0, 3); // adj, source, destination
        for(auto c : path) cout<<char(c - '0' + 'A')<<" ";
        cout<<endl;
    }
    else {
        int ans = max_bandwidth(5, adj, 0, 3);
        for (int node : best_path) cout << char(node + 'A') << " ";
        cout<<endl;
    }

    return 0;
}

