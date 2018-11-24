#pragma once
extern "C" _declspec(dllexport) void _stdcall initSimulation(float input[150][3]);
extern "C" _declspec(dllexport) void _stdcall simulate1(float change1[3]);
extern "C" _declspec(dllexport) void _stdcall simulate2(float change1[3], float change2[3]);
extern "C" _declspec(dllexport) void _stdcall copySimulationResult(float output[]);
