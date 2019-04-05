#include <iostream>
using namespace std;

typedef unsigned char uint_8;

#define MONDAY		1
#define TUESDAY		2
#define WEDNESDAY	3
#define THURSDAY	4
#define FRIDAY		5
#define SATURDAY	6
#define SUNDAY		7

class Time
{
public:
	uint8_t		hour;
	uint8_t		min;
	uint8_t		sec;
	uint8_t		date;
	uint8_t		mon;
	uint16_t	year;
	uint8_t		dow;

	Time() {};
	void setTime(uint8_t h, uint8_t m, uint8_t s, uint8_t d, uint8_t mo, uint16_t y, uint8_t dow);
};

void Time::setTime(uint8_t h, uint8_t m, uint8_t s, uint8_t d, uint8_t mo, uint16_t y, uint8_t dow)
{
		hour = h;
		min = m;
		sec = s;
		date = d;
		mon = mo;
		year = y;
		dow = dow;
}


// Day Of Week Sunday 0 ... Saturday 6
static int dow(Time *ti)
{ 
  if(ti->dow == SUNDAY)
    return 0;
  return ti->dow;
}

uint8_t dst(Time *t)
{
  uint8_t dst = 0; // Normal time
  if(t->mon < 10 && t->mon > 3)  // summer time anyway
    dst = 1;
  else if(t->mon == 3 && (t->date + t->dow > 30)) // After last Sunday in March
    dst = 1;
  else if(t->mon == 10 && (t->date  - dow(t) < 24)) // Before last Sunday in Oct
    dst = 1;
  cout << (int)dst << '\n';
  return dst;
}

int main() 
{
	Time t;

	t.setTime(10, 10, 0, 31, 3, 2019, SUNDAY);
	cout << "Hello, World! " << (int)dst(&t);
    return 0;
}
