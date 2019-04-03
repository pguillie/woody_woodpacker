#include "woody.h"

int woody32(struct file_info *file __attribute__ ((unused)))
{
	return (woody_error(WE_EI_CLASS32, 0));
}
