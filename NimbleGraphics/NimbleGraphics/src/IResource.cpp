#include "IResource.h"

IResource::IResource(IResource&& other)
{

}

IResource& IResource::operator=(IResource&& other)
{
	return *this;
}

IResource::IResource()
{

}