
#include <bits/stdc++.h>
using namespace std;

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");

    if (start == string::npos) {
        return "";
    }

    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string jsonString(const string& s) {
    string result = "\"";

    for (unsigned char c : s) {
        if (c == '"' || c == '\\') {
            result += '\\';
            result += c;
        } else {
            result += c;
        }
    }

    result += '"';
    return result;
}

vector<int> parseDenominations(const string& input) {
    vector<int> coins;
    stringstream ss(input);
    string token;

    while (getline(ss, token, ',')) {
        token = trim(token);

        if (token.empty()) {
            throw runtime_error("Empty coin denomination");
        }

        size_t consumed = 0;
        int coin = stoi(token, &consumed);

        if (consumed != token.size() || coin <= 0) {
            throw runtime_error(
                "Denominations must be positive integers"
            );
        }

        coins.push_back(coin);
    }

    sort(coins.begin(), coins.end());
    coins.erase(
        unique(coins.begin(), coins.end()),
        coins.end()
    );

    if (coins.empty()) {
        throw runtime_error(
            "At least one denomination is required"
        );
    }

    return coins;
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            cerr << "Usage: coin_change_api.exe "
                 << "AMOUNT DENOMINATIONS\n";
            cerr << "Example: coin_change_api.exe 63 \"1,5,10,25\"\n";
            return 1;
        }

        size_t consumed = 0;
        int amount = stoi(argv[1], &consumed);

        if (consumed != string(argv[1]).size() ||
            amount < 0 || amount > 1000000) {
            throw runtime_error(
                "Amount must be an integer from 0 to 1000000"
            );
        }

        vector<int> coins = parseDenominations(argv[2]);

        const int INF = INT_MAX / 2;

        // dp[x] = minimum number of coins needed to make x.
        vector<int> dp(amount + 1, INF);

        // choice[x] stores the last coin used for amount x.
        vector<int> choice(amount + 1, -1);

        dp[0] = 0;

        // Bottom-up Dynamic Programming.
        for (int value = 1; value <= amount; value++) {
            for (int coin : coins) {
                if (coin > value) {
                    break;
                }

                if (dp[value - coin] != INF &&
                    dp[value - coin] + 1 < dp[value]) {
                    dp[value] = dp[value - coin] + 1;
                    choice[value] = coin;
                }
            }
        }

        bool reachable = dp[amount] != INF;
        map<int, int> coinCounts;

        if (reachable) {
            int remaining = amount;

            while (remaining > 0) {
                int coin = choice[remaining];

                if (coin <= 0) {
                    throw runtime_error(
                        "Failed to reconstruct the solution"
                    );
                }

                coinCounts[coin]++;
                remaining -= coin;
            }
        }

        cout << "{";
        cout << "\"success\":true,";
        cout << "\"algorithm\":"
             << jsonString("Dynamic Programming - Coin Change")
             << ",";
        cout << "\"amount\":" << amount << ",";
        cout << "\"denominations\":[";

        for (size_t i = 0; i < coins.size(); i++) {
            if (i > 0) cout << ",";
            cout << coins[i];
        }

        cout << "],";
        cout << "\"reachable\":"
             << (reachable ? "true" : "false") << ",";
        cout << "\"minimum_coins\":";

        if (reachable) {
            cout << dp[amount];
        } else {
            cout << -1;
        }

        cout << ",\"solution\":[";

        bool first = true;

        for (const auto& entry : coinCounts) {
            if (!first) cout << ",";
            first = false;

            cout << "{";
            cout << "\"denomination\":" << entry.first << ",";
            cout << "\"count\":" << entry.second;
            cout << "}";
        }

        cout << "],";
        cout << "\"time_complexity\":\"O(amount * n)\",";
        cout << "\"space_complexity\":\"O(amount)\"";
        cout << "}\n";

        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
