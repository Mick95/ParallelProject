#include "stdafx.h"
#include "VM.h"
#include <iostream>


VM::VM()
{
}


VM::VM(int id, float gamma, float d_i, float chi_i, float chi_0) {
	i = id;
	gamma_i = gamma;
	deadLine_i = d_i;
	this->chi_i = chi_i;
	this->chi_0 = chi_0;
	t1 = 0;
	t2 = 0;
}

int VM::getID() {
	return i;
}

float VM::getGamma() {
	return gamma_i;
}

float VM::getDeadLine() {
	return deadLine_i;
}

float VM::getVM() {
	return vm;
}

float VM::getVMLastIteration() {
	return vmOld;
}

float VM::getTemporaryCore(float deltaVM) {
	return floor((deltaVM + vm)*gamma_i);
}

float VM::getCore() {
	return core_i;
}

float VM::getA() {
	return a;
}

float VM::getB() {
	return b;
}

float VM::getT1() {
	return t1;
}

float VM::getT2() {
	return t2;
}

float VM::getChi_i() {
	return chi_i;
}

float VM::getChi_0() {
	return chi_0;
}

bool VM::getCondition() {
	return optimizationDone;
}

void VM::setID(int id) {
	i = id;
}

void VM::setGamma(float gamma){
		gamma_i = gamma;
	}

void VM::setDeadLine(float deadLine) {
	this->deadLine_i = deadLine;
}

void VM::setVM(float vm) {
	vm = vm;
}

void VM::setVM_UsingCore() {
	vm = core_i / gamma_i;
	vm = ceil(vm);
}

void VM::setVMLastIteration(float num) {
	vmOld = num;
}

void VM::setCore(float core) {
	core_i = core;
}

void VM::setCoreAfterSimulator(float core) {
	core_i = floor((core + vm)*gamma_i);
}

void VM::setA(float t1) {
	a = t1;
}

void VM::setB(float t2) {
	b = t2;
}

void VM::setT1(float t1) {
	this->t1 = t1;
}

void VM::setT2(float t2) {
	this->t2 = t2;
}

void VM::setChi_0(float chi_0){
	this->chi_0 = chi_0;
}

void VM::setChi_i(float chi_i) {
	this->chi_i = chi_i;
}

void VM::setCondition(bool cond) {
	optimizationDone = cond;
}
