/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : ctor3.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

class Animal
{
protected:
	Animal() {}
};

class Dog : public Animal
{
public:
	Dog() : Animal() {}
};

int main()
{
	// ������ ������ ��� ��� ������.
	Animal a; // error.
	Dog    d; // ok..
}