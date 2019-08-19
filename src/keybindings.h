#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <unordered_map>
#include <tuple>
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <assert.h>
#include <iostream>
#include <bitset>

#include <appcontrols.h>
#include <app.h>

void setupBindings();
void runTests();
void BIND(int32_t key, void(*func)(App) );
void BIND( std::tuple<int32_t, int32_t> key, void(*func)(App) );
constexpr int64_t int64FromInt32(int32_t a, int32_t b);
constexpr int32_t int32FromInt64(int64_t val, int32_t offset);

extern std::unordered_map<int64_t, void(*)(App)> pressBindings;

#endif // KEYBINDINGS_H
