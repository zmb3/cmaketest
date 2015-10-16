#include "lib.h"

#include <iostream>

#include "pcap/pcap.h"

void Foo::bar()
{
   pcap_t* cap = pcap_open_dead(0, 32);
   if (cap)
   {
      std::cout << "got pcap" << std::endl;
      pcap_close(cap);
   }
   else
   {
      std::cout << "didn't get pcap" << std::endl;
   }
}
