#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

using Clause = set<int>;
using ClauseSet = vector<Clause>;

ClauseSet readFormulaFromFile(const string& filename) {
    ClauseSet formula;
    ifstream file(filename);
    if (!file) {
        cerr << "Eroare la deschiderea fisierului!" << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == 'c') continue;
        if (line[0] == 'p') continue;

        stringstream ss(line);
        Clause clause;
        int lit;
        while (ss >> lit) {
            if (lit == 0) break;
            clause.insert(lit);
        }
        if (!clause.empty())
            formula.push_back(clause);
    }

    return formula;
}

void printClause(const Clause& c) {
    cout << "{ ";
    for (int lit : c)
        cout << lit << " ";
    cout << "}";
}

Clause resolve(const Clause& c1, const Clause& c2) {
    for (int lit : c1) {
        if (c2.count(-lit)) {
            Clause resolvent;
            for (int l : c1)
                if (l != lit) resolvent.insert(l);
            for (int l : c2)
                if (l != -lit) resolvent.insert(l);
            return resolvent;
        }
    }
    return {};
}

bool resolution(ClauseSet& formula) {
    unordered_set<string> seenClauses;
    ClauseSet newClauses;

    for (const Clause& clause : formula) {
        string clauseStr;
        for (int lit : clause)
            clauseStr += to_string(lit) + " ";
        seenClauses.insert(clauseStr);
    }

    while (true) {
        bool newClauseAdded = false;

        for (size_t i = 0; i < formula.size(); ++i) {
            for (size_t j = i + 1; j < formula.size(); ++j) {

                Clause resolvent = resolve(formula[i], formula[j]);
                if (resolvent.empty()) {
                    cout << "\n>> Clauza vida a fost dedusa din: ";
                    printClause(formula[i]);
                    cout << " si ";
                    printClause(formula[j]);
                    cout << endl;
                    return false;
                }

                string resolventStr;
                for (int lit : resolvent)
                    resolventStr += to_string(lit) + " ";
                if (!resolvent.empty() && seenClauses.count(resolventStr) == 0) {
                    seenClauses.insert(resolventStr);
                    newClauses.push_back(resolvent);
                    newClauseAdded = true;
                }
            }
        }

        if (!newClauseAdded)
            return true;

        for (const auto& c : newClauses)
            formula.push_back(c);

        cout << "Numar total de clauze dupa iteratie: " << formula.size() << endl;
        newClauses.clear();
    }
}

int main() {
    ClauseSet formula = readFormulaFromFile("input.txt");

    bool result = resolution(formula);

    cout << "\n=== Rezultat ===\n";
    if (result)
        cout << "SATISFIABILA" << endl;
    else
        cout << "NESATISFIABILA" << endl;

    cout << "Numar total de clauze (initiale + deduse): " << formula.size() << endl;

    return 0;
}
