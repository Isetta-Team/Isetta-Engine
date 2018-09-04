#pragma once
	
class IModule
{
public:
    virtual ~IModule() {}
    virtual void StartUp() = 0;
    virtual void ShutDown() = 0;
};