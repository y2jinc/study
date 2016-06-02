#pragma once

struct IGraph
{
	virtual void onUpdate(int n) = 0;
	virtual ~IGraph() {}
};
