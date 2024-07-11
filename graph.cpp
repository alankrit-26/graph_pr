#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

using namespace std;

class Graph {
public:
    class Vertex {
    public:
        map<string, int> nbrs;
    };

    unordered_map<string, Vertex> vtces;

    int numVertex() {
        return this->vtces.size();
    }

    bool containsVertex(string vname) {
        return this->vtces.find(vname) != this->vtces.end();
    }

    void addVertex(string vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(string vname) {
        Vertex vtx = vtces[vname];
        vector<string> keys;
        for (const auto& kv : vtx.nbrs) {
            keys.push_back(kv.first);
        }

        for (const string& key : keys) {
            Vertex& nbrVtx = vtces[key];
            nbrVtx.nbrs.erase(vname);
        }

        vtces.erase(vname);
    }

    int numEdges() {
        int count = 0;
        for (const auto& kv : vtces) {
            count += kv.second.nbrs.size();
        }
        return count / 2;
    }

    bool containsEdge(string vname1, string vname2) {
        Vertex vtx1 = vtces[vname1];
        Vertex vtx2 = vtces[vname2];

        if (vtx1.nbrs.find(vname2) == vtx1.nbrs.end()) {
            return false;
        }

        return true;
    }

    void addEdge(string vname1, string vname2, int value) {
        Vertex& vtx1 = vtces[vname1];
        Vertex& vtx2 = vtces[vname2];

        if (vtx1.nbrs.find(vname2) != vtx1.nbrs.end()) {
            return;
        }

        vtx1.nbrs[vname2] = value;
        vtx2.nbrs[vname1] = value;
    }

    void removeEdge(string vname1, string vname2) {
        Vertex& vtx1 = vtces[vname1];
        Vertex& vtx2 = vtces[vname2];

        if (vtx1.nbrs.find(vname2) == vtx1.nbrs.end()) {
            return;
        }

        vtx1.nbrs.erase(vname2);
        vtx2.nbrs.erase(vname1);
    }

    void displayMap() {
        cout << "\t Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------" << endl;

        for (const auto& kv : vtces) {
            string str = kv.first + " =>\n";
            Vertex vtx = kv.second;
            for (const auto& nbr : vtx.nbrs) {
                str += "\t" + nbr.first + "\t";
                if (nbr.first.length() < 16)
                    str += "\t";
                if (nbr.first.length() < 8)
                    str += "\t";
                str += to_string(nbr.second) + "\n";
            }
            cout << str;
        }
        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------" << endl;
    }

    void displayStations() {
        cout << "\n***********************************************************************\n";
        int i = 1;
        for (const auto& kv : vtces) {
            cout << i << ". " << kv.first << endl;
            i++;
        }
        cout << "\n***********************************************************************\n";
    }

    bool hasPath(string vname1, string vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2)) {
            return true;
        }

        processed[vname1] = true;
        Vertex vtx = vtces[vname1];

        for (const auto& nbr : vtx.nbrs) {
            if (processed.find(nbr.first) == processed.end())
                if (hasPath(nbr.first, vname2, processed))
                    return true;
        }

        return false;
    }

    class DijkstraPair {
    public:
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair& o) const {
            return cost > o.cost;
        }
    };

    int dijkstra(string src, string des, bool nan) {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair> map;
        priority_queue<DijkstraPair> heap;

        for (const auto& kv : vtces) {
            DijkstraPair np;
            np.vname = kv.first;
            np.cost = INT_MAX;

            if (kv.first == src) {
                np.cost = 0;
                np.psf = kv.first;
            }

            heap.push(np);
            map[kv.first] = np;
        }

        while (!heap.empty()) {
            DijkstraPair rp = heap.top();
            heap.pop();

            if (rp.vname == des) {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);
            ans.push_back(rp.vname);

            Vertex v = vtces[rp.vname];
            for (const auto& nbr : v.nbrs) {
                if (map.find(nbr.first) != map.end()) {
                    int oc = map[nbr.first].cost;
                    Vertex k = vtces[rp.vname];
                    int nc = nan ? rp.cost + 120 + 40 * k.nbrs[nbr.first] : rp.cost + k.nbrs[nbr.first];

                    if (nc < oc) {
                        DijkstraPair& gp = map[nbr.first];
                        gp.psf = rp.psf + nbr.first;
                        gp.cost = nc;

                        heap.push(gp);
                    }
                }
            }
        }
        return val;
    }

    class Pair {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    string getMinimumDistance(string src, string dst) {
        int min = INT_MAX;
        string ans;
        unordered_map<string, bool> processed;
        vector<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_back(sp);

        while (!stack.empty()) {
            Pair rp = stack.back();
            stack.pop_back();

            if (processed.find(rp.vname) != processed.end()) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                if (rp.min_dis < min) {
                    ans = rp.psf;
                    min = rp.min_dis;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (const auto& nbr : rpvtx.nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_dis = rp.min_dis + rpvtx.nbrs[nbr.first];
                    stack.push_back(np);
                }
            }
        }
        ans += to_string(min);
        return ans;
    }

    string getMinimumTime(string src, string dst) {
        int min = INT_MAX;
        string ans;
        unordered_map<string, bool> processed;
        vector<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;

        stack.push_back(sp);

        while (!stack.empty()) {
            Pair rp = stack.back();
            stack.pop_back();

            if (processed.find(rp.vname) != processed.end()) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                if (rp.min_time < min) {
                    ans = rp.psf;
                    min = rp.min_time;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (const auto& nbr : rpvtx.nbrs) {
                if (processed.find(nbr.first) == processed.end()) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_time = rp.min_time + 120 + 40 * rpvtx.nbrs[nbr.first];
                    stack.push_back(np);
                }
            }
        }
        double minutes = ceil(static_cast<double>(min) / 60);
        ans += to_string(minutes);
        return ans;
    }

    vector<string> getInterchanges(string str) {
        vector<string> arr;
        stringstream ss(str);
        string token;
        vector<string> res;

        while (getline(ss, token, ' ')) {
            if (!token.empty()) {
                res.push_back(token);
            }
        }

        arr.push_back(res[0]);
        int count = 0;
        for (size_t i = 1; i < res.size() - 1; ++i) {
            size_t index = res[i].find('~');
            if (index != string::npos) {
                string temp1 = res[i].substr(0, index);
                string temp2 = res[i].substr(index + 1);
                if (temp1 != temp2) {
                    arr.push_back(res[i]);
                    count++;
                }
            } else {
                arr.push_back(res[i]);
            }
        }
        arr.push_back(to_string(count));
        arr.push_back(res.back());

        return arr;
    }

    void createMetroMap(Graph& g) {
        g.addVertex("Noida Sector 62");
        g.addVertex("Botanical Garden");
        g.addVertex("Yamuna Bank");
        g.addVertex("Rajiv Chowk");
        g.addVertex("Vaishali");
        g.addVertex("Moti Nagar");
        g.addVertex("Janak Puri West");
        g.addVertex("Dwarka Sector 21");
        g.addVertex("Huda City Center");
        g.addVertex("Saket");
        g.addVertex("AIIMS");
        g.addVertex("Chandni Chowk");
        g.addVertex("New Delhi");
        g.addVertex("Shivaji Stadium");
        g.addVertex("Punjabi Bagh West");
        g.addVertex("Netaji Subhash Place");
        g.addVertex("Azadpur");
        g.addVertex("Badli");
        g.addVertex("Brigadier Hoshiar Singh");
        g.addVertex("Bahadurgarh City");
        g.addVertex("Mundka Industrial Area");
        g.addVertex("Punjabi Bagh East");

        g.addEdge("Noida Sector 62", "Botanical Garden", 8);
        g.addEdge("Botanical Garden", "Yamuna Bank", 10);
        g.addEdge("Yamuna Bank", "Rajiv Chowk", 6);
        g.addEdge("Yamuna Bank", "Vaishali", 8);
        g.addEdge("Rajiv Chowk", "Moti Nagar", 9);
        g.addEdge("Moti Nagar", "Janak Puri West", 7);
        g.addEdge("Janak Puri West", "Dwarka Sector 21", 10);
        g.addEdge("Huda City Center", "Saket", 15);
        g.addEdge("Saket", "AIIMS", 5);
        g.addEdge("AIIMS", "Rajiv Chowk", 7);
        g.addEdge("Chandni Chowk", "New Delhi", 2);
        g.addEdge("New Delhi", "Shivaji Stadium", 2);
        g.addEdge("Punjabi Bagh West", "Netaji Subhash Place", 2);
        g.addEdge("Netaji Subhash Place", "Azadpur", 2);
        g.addEdge("Azadpur", "Badli", 2);
        g.addEdge("Brigadier Hoshiar Singh", "Bahadurgarh City", 2);
        g.addEdge("Bahadurgarh City", "Mundka Industrial Area", 2);
        g.addEdge("Mundka Industrial Area", "Punjabi Bagh East", 2);
        g.addEdge("Punjabi Bagh East", "Punjabi Bagh West", 2);
    }
};

int main() {
    Graph g;
    g.createMetroMap(g);
    g.displayMap();
cout << "\t\t\t****WELCOME TO THE METRO APP*****" << endl;

    while (true) {
        cout << "\t\t\t\t~~LIST OF ACTIONS~~\n\n";
        cout << "1. LIST ALL THE STATIONS IN THE MAP\n";
        cout << "2. SHOW THE METRO MAP\n";
        cout << "3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "7. EXIT THE MENU\n";
        cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 7) : ";

        int choice;
        cin >> choice;
        cin.ignore();  // Clear input buffer

        switch (choice) {
            case 1:
                g.display_Stations();
                break;

            case 2:
                g.display_Map();
                break;

            case 3: {
                vector<string> keys;
                for (const auto &entry : g.vtces) {
                    keys.push_back(entry.first);
                }

                string* codes = printCodelist();
                cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER CODE OF STATIONS\n3. TO ENTER NAME OF STATIONS\n";
                cout << "ENTER YOUR CHOICE:";
                int ch;
                cin >> ch;
                cin.ignore();

                string st1, st2;
                cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
                if (ch == 1) {
                    int serial1, serial2;
                    cin >> serial1 >> serial2;
                    cin.ignore();
                    st1 = keys[serial1 - 1];
                    st2 = keys[serial2 - 1];
                } else if (ch == 2) {
                    string a, b;
                    cout << "Enter code for source station: ";
                    getline(cin, a);
                    transform(a.begin(), a.end(), a.begin(), ::toupper);
                    auto it1 = find_if(begin(codes), end(codes), [&a](const string& s) { return s == a; });
                    st1 = keys[distance(codes, it1)];

                    cout << "Enter code for destination station: ";
                    getline(cin, b);
                    transform(b.begin(), b.end(), b.begin(), ::toupper);
                    auto it2 = find_if(begin(codes), end(codes), [&b](const string& s) { return s == b; });
                    st2 = keys[distance(codes, it2)];
                } else if (ch == 3) {
                    cout << "Enter name for source station: ";
                    getline(cin, st1);
                    cout << "Enter name for destination station: ";
                    getline(cin, st2);
                } else {
                    cout << "Invalid choice" << endl;
                    exit(0);
                }

                unordered_map<string, bool> processed;
                if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed)) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                } else {
                    cout << "SHORTEST DISTANCE FROM " << st1 << " TO " << st2 << " IS " << g.dijkstra(st1, st2, false) << " KM" << endl;
                }
                break;
            }

            case 4: {
                string sat1, sat2;
                cout << "ENTER THE SOURCE STATION: ";
                getline(cin, sat1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, sat2);

                unordered_map<string, bool> processed1;
                cout << "SHORTEST TIME FROM (" << sat1 << ") TO (" << sat2 << ") IS " << g.dijkstra(sat1, sat2, true) / 60 << " MINUTES" << endl;
                break;
            }

            case 5: {
                string s1, s2;
                cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
                cout << "Source station: ";
                getline(cin, s1);
                cout << "Destination station: ";
                getline(cin, s2);

                unordered_map<string, bool> processed2;
                if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2)) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                } else {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                    int len = str.size();
                    cout << "SOURCE STATION : " << s1 << endl;
                    cout << "DESTINATION STATION : " << s2 << endl;
                    cout << "DISTANCE : " << str[len - 1] << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                    cout << "~~~~~~~~~~~~~" << endl;
                    cout << "START  ==>  " << str[0] << endl;
                    for (int i = 1; i < len - 3; i++) {
                        cout << str[i] << endl;
                    }
                    cout << str[len - 3] << "   ==>    END" << endl;
                    cout << "~~~~~~~~~~~~~" << endl;
                }
                break;
            }

            case 6: {
                string ss1, ss2;
                cout << "ENTER THE SOURCE STATION: ";
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3)) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                } else {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(ss1, ss2));
                    int len = str.size();
                    cout << "SOURCE STATION : " << ss1 << endl;
                    cout << "DESTINATION STATION : " << ss2 << endl;
                    cout << "TIME : " << str[len - 1] << " MINUTES" << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                    cout << "START  ==>  " << str[0] << " ==>  ";
                    for (int i = 1; i < len - 3; i++) {
                        cout << str[i] << endl;
                    }
                    cout << str[len - 3] << "   ==>    END" << endl;
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                }
                break;
            }

            case 7:
                cout << "Exiting the menu..." << endl;
                exit(0);

            default:
                cout << "Please enter a valid option (1-7)." << endl;
                break;
        }

        cout << "\n***********************************************************\n";
    }

    return 0;
}
