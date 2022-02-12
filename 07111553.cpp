//#include "Hamilton.h"
#include <fstream>
#include <strstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

#define INF pow(10,9)

using namespace std;

// Compare statement to execute
bool STR_CMP(const char* str1, const char* str2) {
	int i = 0;
	while (str1[i] != '\0') {
		if (str1[i] != str2[i]) return false;
		i++;
	}
	return str1[i] == '\0';
}

// Go to next vertex (in Hamilton Cycle)
void NextVertex(int* x, int k, int n, bool** G) {
	do {
		x[k] = (x[k] + 1) % (n + 1);
		//do the loop until visited all vertex
		if (x[k] == 0)
			// If x[k]==0 --> entire n vertex were visited
			return;

		if (G[x[k - 1]][x[k]]) {
			//	If the edge between this vertex and 
			int j;
			for (j = 1; j < k; j++) {
				if (x[j] == x[k])
					break;
			}
			//Check duplicate
			// If this vertex was visited --> j < k --> continue the loop do while
			if (j == k)
				//If i==k then the vertex k is not visited
				if (k < n || (k == n && G[x[n]][x[1]]))
					// If:
					// k < n then return because this vertex is not visited
					// k==n and G[x[n]][x[1]] = true --> the cycle is complete --> exist a Hamilton Cycle
					return;
		}
	} while (true);
}

// Hamilton Cycle
void Hamilton_Cycle(int* x, int k, int n, bool** G, bool& check) {
	if (check)
		return;
	do {
		NextVertex(x, k, n, G);
		//Next vertex 
		if (x[k] == 0)
			return;
		//If visited whole n vertex --> return
		if (k == n)
			check = true;
		//If exist a Hamilton Cycle --> return
		else
			Hamilton_Cycle(x, k + 1, n, G, check);
		//Continue the Hamilton Check if k < n
	} while (true);
}

// Next vertex (in Hamilton Path)
void NextVertex_Path(int* x, int k, int n, bool** G) {
	do {
		x[k] = (x[k] + 1) % (n + 1);
		//do the loop until visited all vertex
		if (x[k] == 0)
			// If x[k]==0 --> entire n vertex were visited
			return;

		if (G[x[k - 1]][x[k]]) {
			//	If the edge between this vertex and 
			int j;
			for (j = 1; j < k; j++) {
				if (x[j] == x[k])
					break;
			}
			//Check duplicate
			// If this vertex was visited --> j < k --> continue the loop do while
			if (j == k)
				//If i==k then the vertex k is not visited
				// Don't need check the cycle
				return;
		}
	} while (true);
}

// Hamilton Path
void Hamilton_Path(int* x, int k, int n, bool** G, bool& check) {
	if (check)
		return;
	do {
		NextVertex(x, k, n, G);
		//Next vertex 
		if (x[k] == 0)
			return;
		//If visited whole n vertex --> return
		if (k == n - 1)
			check = true;
		//If exist a Hamilton Cycle --> return
		else
			Hamilton_Path(x, k + 1, n, G, check);
		//Continue the Hamilton Check if k < n
	} while (true);
}

// Travelling Salesman
int TSP_Man(int start, int mask, int pos, int n, bool** T, int** G, int VISITED_ALL, int** DP) {
	if (mask == VISITED_ALL) {
		if (T[pos][start]) {
			// If all of vertex are visited then return the final vertex to end cycle
			DP[mask][pos] = G[pos][start];
			return G[pos][start];
		}
		else {
			DP[mask][pos] = INF;
			return INF;
		}
	}
	if (DP[mask][pos] != -1) {
		// If the vertex is visited then skip it
		return DP[mask][pos];
	}
	int ans = INF;
	for (int i = 0; i < n; i++) {
		if (T[pos][i] && ((mask & (1 << i)) == 0)) {
			//Find the cycle have min weight
			//1 << pos: The value of this calculation is 2^n
			//For example: (1 << 3) --> 1000 --> 8
			// The value binary of it to check the vertex which are visited
			//The bit |: to check the vertex are visited by bit or ( "|" )
			// If the vertex are 1, then visited vertex 3
			// Now, the mask is (1|(1<<3)) <---> (001|1000) <--> (1001)
			//The meaning of value "1001" that:
			//The vertex 1 and 3 are visited
			int MID = TSP_Man(start, mask | (1 << i), i, n, T, G, VISITED_ALL, DP) + G[pos][i];
			if (ans > MID) {
				ans = MID;
			}
		}
	}
	DP[mask][pos] = ans;
	return ans;
}

//Delete an array bool dimension N x N
void Del_Arr_Bool(bool** arr, int n) {
	for (int i = 0; i < n; i++) {
		delete[]arr[i];
	}
	delete[]arr;
}

//Delete an array int dimension N x N
void Del_Arr_Int(int** arr, int n) {
	for (int i = 0; i < n; i++) {
		delete[]arr[i];
	}
	delete[]arr;
}

//Create an array bool dimension N x N
bool** Create_T(int n) {
	bool** res = new bool* [n];
	for (int i = 0; i < n; i++) {
		res[i] = new bool[n];
		for (int j = 0; j < n; j++)
			res[i][j] = false;
	}
	return res;
}

//Create an array int dimension N x N
int** Create_G(int n, int key) {
	int** res = new int* [n];
	for (int i = 0; i < n; i++) {
		res[i] = new int[n];
		for (int j = 0; j < n; j++) {
			res[i][j] = key;
		}
	}
	return res;
}

//Print the path of TSP
void Print(int mask, int pos, int n, bool** T, int** G, int** DP, int VISITED_ALL, bool& check) {
	// Start the path from node 1
	if (mask == 1) {
		cout << "1 ";
	}

	// The variable check is condition to break when you find result.
	// This is because you just need find only one path
	if (mask == VISITED_ALL) {
		check = true;
		// Mark to stop the recursion
		return;
	}
	for (int i = 0; i < n; i++) {
		//Same as the TSP function, Print function check if:
		// T[pos][i]									  : Exist edge from pos to i
		// mask & (1 << i)								  : The vertex wasn't visited
		// DP[mask][pos] - DP[mask|(1 << i)][i]==G[pos][i]: The fee go to that vertex is equal to the fee to previous vertex minus the weight to go from pos to i
		if (T[pos][i] && DP[mask][pos] - DP[mask | (1 << i)][i] == G[pos][i] && (mask & (1 << i)) == 0) {
			//Print the result
			cout << (i + 1) << " ";
			//Recursive to check the next vertex
			Print(mask | (1 << i), i, n, T, G, DP, VISITED_ALL, check);
		}
		if (check) {
			return;
		}
	}
}

// Execute hamilton path and print the result
void Execute_Hamilton_Path(int n, vector<vector<int>>input) {
	bool** T = Create_T(n + 1);
	for (int i = 0; i < input.size(); i++) {
		T[input[i][0]][input[i][1]] = true;
		T[input[i][1]][input[i][0]] = true;
	}
	int* x = new int[n + 1];
	for (int i = 0; i <= n; i++)
		x[i] = 0;
	T[0][1] = true;
	bool check = false;
	Hamilton_Path(x, 1, n, T, check);
	if (check || n == 1) cout << "YES\n";
	else cout << "NO\n";
	Del_Arr_Bool(T, n + 1);
}

//Execute hamilton cycle and print the result
void Execute_Hamilton_Cycle(int n, vector<vector<int>>input) {
	bool** T = Create_T(n + 1);
	for (int i = 0; i < input.size(); i++) {
		T[input[i][0]][input[i][1]] = true;
		T[input[i][1]][input[i][0]] = true;
	}
	int* x = new int[n + 1];
	for (int i = 0; i <= n; i++)
		x[i] = 0;
	T[0][1] = true;
	bool check = false;
	Hamilton_Cycle(x, 1, n, T, check);
	if (check || n == 1) cout << "YES\n";
	else cout << "NO\n";
	Del_Arr_Bool(T, n + 1);
}

//Execute the TSP and print the result
void Execute_TSP_Man(int n, vector<vector<int>>input) {
	bool** T = Create_T(n);
	int** G = Create_G(n, -1);
	for (int i = 0; i < input.size(); i++) {
		T[input[i][0] - 1][input[i][1] - 1] = true;
		T[input[i][1] - 1][input[i][0] - 1] = true;
		G[input[i][0] - 1][input[i][1] - 1] = input[i][2];
		G[input[i][1] - 1][input[i][0] - 1] = input[i][2];
	}
	int VISITED_ALL = (1 << n) - 1;
	int** DP = new int* [(1 << n)];
	for (int i = 0; i < (1 << n); i++) {
		DP[i] = new int[n];
		for (int j = 0; j < n; j++)
			DP[i][j] = -1;
	}
	int ANS = TSP_Man(0, 1, 0, n, T, G, VISITED_ALL, DP);
	if (ANS < INF) {
		cout << ANS << endl;
		bool check = false;
		Print(1, 0, n, T, G, DP, VISITED_ALL, check);
	}
	else {
		cout << -1 << endl;
	}
	Del_Arr_Bool(T, n);
	Del_Arr_Int(G, n);
	for (int i = 0; i < (1 << n); i++) {
		delete[]DP[i];
	}
	delete[]DP;
}

int main(int args, char* argv[]) {
	//--TODO--
	char* Act = argv[1];
	ifstream f(argv[2]);
	int n;
	string chuoi;
	getline(f, chuoi);
	n = stoi(chuoi);
	vector<vector<int>>input;
	stringstream ss;
	do{
		vector<int>Line;
		getline(f, chuoi);
		if (f.eof() && chuoi.length() == 0) {
			break;
		}
		ss << chuoi;
		while (true) {
			int x;
			ss >> x;
			if (!ss) {
				break;
			}
			Line.push_back(x);
		}
		ss.clear();
		input.push_back(Line);
	} while (!f.eof());
	if (STR_CMP(Act,"-HPath")) {
		Execute_Hamilton_Path(n,input);
	}
	else if (STR_CMP(Act, "-HCycle")) {
		Execute_Hamilton_Cycle(n,input);
	}
	else if (STR_CMP(Act, "-TSP")) {
		Execute_TSP_Man(n,input);
	}
	else {
		cout << "Invalid Action\n";
	}
	f.close();
	return 0;
}