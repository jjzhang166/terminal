/*
 * Scrolling.h
 *
 *  Created on: 2012-7-27
 *      Author: Administrator
 */

#ifndef SCROLLING_H_
#define SCROLLING_H_

#include <windows.h>
#include "../util/Runnable.h"
#include "../util/ThreadCreator.h"

class Scrolling: public Runnable {
private:
	bool running;
public:
	Scrolling(void);
	virtual ~Scrolling(void);
	BOOL StartScrolling(HDC hdc, HDC m_dcMemoryDC, char* szText, int len,
			LPSIZE s, int width1, int heigth1, LPRECT rect,
			HBRUSH m_hBrushBackground);
	void Run();
	void Stop();
};

#endif /* SCROLLING_H_ */
