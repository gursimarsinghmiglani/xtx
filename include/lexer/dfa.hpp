#pragma once
#include "automata_config.hpp"
#include "dsu.hpp"
#include "token.hpp"
#include "token_priority.hpp"
#include <stack>
#include <string>
#include <vector>
struct DFA {
  int num_states;
  int start_state;
  std::vector<std::vector<int>> delta;
  std::vector<Token> token_types;
  DFA(int num_states, int start_state,
      const std::vector<std::vector<int>> &delta,
      const std::vector<Token> token_types)
      : num_states(num_states), start_state(start_state), delta(delta),
        token_types(token_types) {}
  DFA minimize() const {
    std::vector<bool> visited(num_states);
    std::stack<int> stack;
    stack.push(start_state);
    visited[start_state] = true;
    while (!stack.empty()) {
      int u = stack.top();
      stack.pop();
      for (int alpha = 0; alpha < ALPHABET_SIZE; alpha++) {
        int v = delta[u][alpha];
        if (!visited[v]) {
          visited[v] = true;
          stack.push(v);
        }
      }
    }
    int cnt = 0;
    std::vector<int> mapping(num_states, -1);
    for (int i = 0; i < num_states; i++) {
      if (visited[i])
        mapping[i] = cnt++;
    }
    int start_state_updated = mapping[start_state];
    std::vector<std::vector<int>> delta_updated(
        cnt, std::vector<int>(ALPHABET_SIZE));
    for (int i = 0; i < num_states; i++) {
      for (int alpha = 0; alpha < ALPHABET_SIZE; alpha++) {
        if (mapping[i] != -1) {
          delta_updated[mapping[i]][alpha] = mapping[delta[i][alpha]];
        }
      }
    }
    std::vector<Token> token_types_updated(cnt, Token::TOK_ERROR);
    for (int i = 0; i < num_states; i++) {
      if (mapping[i] != -1) {
        token_types_updated[mapping[i]] = token_types[i];
      }
    }
    std::vector<std::vector<int>> table(cnt, std::vector<int>(cnt));
    for (int i = 0; i < cnt; i++) {
      for (int j = 0; j < cnt; j++) {
        if (token_types_updated[i] != token_types_updated[j]) {
          table[i][j] = 1;
          table[j][i] = 1;
        }
      }
    }
    int change;
    do {
      change = 0;
      for (int i = 0; i < cnt; i++) {
        for (int j = 0; j < cnt; j++) {
          for (int alpha = 0; alpha < ALPHABET_SIZE; alpha++) {
            if (!table[i][j] &&
                table[delta_updated[i][alpha]][delta_updated[j][alpha]]) {
              table[i][j] = 1;
              table[j][i] = 1;
              change = 1;
            }
          }
        }
      }
    } while (change);
    DSU dsu(cnt);
    for (int i = 0; i < cnt; i++) {
      for (int j = 0; j < cnt; j++) {
        if (!table[i][j]) {
          dsu.union_sets(i, j);
        }
      }
    }
    int num_states_new = dsu.num_sets;
    std::vector<int> mapping_new(cnt, -1);
    int cnt2 = 0;
    for (int i = 0; i < cnt; i++) {
      if (mapping_new[dsu.find(i)] == -1) {
        mapping_new[dsu.find(i)] = cnt2++;
      }
    }
    int start_state_new = mapping_new[dsu.find(start_state_updated)];
    std::vector<std::vector<int>> delta_new(num_states_new,
                                            std::vector<int>(ALPHABET_SIZE));
    for (int i = 0; i < cnt; i++) {
      for (int alpha = 0; alpha < ALPHABET_SIZE; alpha++) {
        delta_new[mapping_new[dsu.find(i)]][alpha] =
            mapping_new[dsu.find(delta_updated[i][alpha])];
      }
    }
    std::vector<Token> token_types_new(num_states_new, Token::TOK_ERROR);
    for (int i = 0; i < cnt; i++) {
      token_types_new[mapping_new[dsu.find(i)]] = max(
          token_types_new[mapping_new[dsu.find(i)]], token_types_updated[i]);
    }
    DFA dfa_new(num_states_new, start_state_new, delta_new, token_types_new);
    return dfa_new;
  }
  Token token(const std::string &str) const {
    int state = start_state;
    for (int i = 0; i < str.length(); i++) {
      state = delta[state][static_cast<unsigned char>(str[i])];
    }
    return token_types[state];
  }
};
