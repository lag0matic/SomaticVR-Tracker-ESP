#include <cassert>
#include <cstdio>
#define PIN_ENABLE_LATCH 2
#include "../../src/PowerLatch.h"
#include "../../lib/FUSB302/FUSB302_UFP.cpp"
static bool unstable=true, failure=false;
static unsigned reads=0, delays=0;
static FUSB302_ret_t readMock(uint8_t,uint8_t a,uint8_t* b,uint8_t n) {
 reads++; if(failure)return FUSB302_ERR_READ_DEVICE;
 for(unsigned i=0;i<n;i++)b[i]=0;
 if(a==0x40)b[0]=0x80 | (unstable ? (reads%2?1:2) : 1);
 return FUSB302_SUCCESS;
}
static FUSB302_ret_t writeMock(uint8_t,uint8_t,uint8_t*,uint8_t) {return FUSB302_SUCCESS;}
static FUSB302_ret_t delayMock(uint32_t n) {delays+=n;return FUSB302_SUCCESS;}
int main() {
 using namespace SlimeVR;
 assert(!shouldLatchAtStartup(false,ESP_RST_POWERON));
 assert(!shouldLatchAtStartup(false,ESP_RST_BROWNOUT));
 assert(!shouldLatchAtStartup(false,ESP_RST_DEEPSLEEP));
 assert(!shouldLatchAtStartup(false,ESP_RST_UNKNOWN));
 for(auto r:{ESP_RST_SW,ESP_RST_PANIC,ESP_RST_INT_WDT,ESP_RST_TASK_WDT,ESP_RST_WDT,ESP_RST_USB,ESP_RST_JTAG})assert(shouldLatchAtStartup(false,r));
 assert(shouldLatchAtStartup(true,ESP_RST_POWERON));
 for(bool level:{false,true}) {
  operations.clear();setPowerLatch(level);
  assert(operations==std::vector<int>({level?1:0,2,3,4}));
  assert(held && pad==level);
 }
 FUSB302_dev_t dev{};dev.i2c_read=readMock;dev.i2c_write=writeMock;dev.delay_ms=delayMock;
 FUSB302_event_t events=0;
 assert(FUSB302_state_unattached(&dev,&events)==FUSB302_BUSY);
 assert(reads<20 && delays<=3 && events==0);
 failure=true;reads=0;
 assert(FUSB302_state_unattached(&dev,&events)==FUSB302_ERR_READ_DEVICE && reads==1);
 failure=false;unstable=false;
 assert(FUSB302_state_unattached(&dev,&events)==FUSB302_SUCCESS && (events&FUSB302_EVENT_ATTACHED));
 puts("PASS: startup reset policy, latch high/low transitions and operation order, bounded PD errors, PD recovery");
}
