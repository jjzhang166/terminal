/*
 * DrawRate.h
 *
 *  Created on: 2012-7-16
 *      Author: Administrator
 */

#ifndef DRAWRATE_H_
#define DRAWRATE_H_

#include "../util/Window.h"
#include "../util/LoopRunnable.h"
#include <string>
using namespace std;

class DrawRate: public Window, public LoopRunnable {
private:
	string bmpFile;
	string posFile;
	string section;
	string desfile;
	int interval;

public:
	DrawRate(string title);

	virtual ~DrawRate();

	void OnPaint(HDC hDC);

	void SetBmpFile(string bmpFile);

	void SetSection(string section);

	void DrawComplete(string lpAppName, HDC hdc);

	void DrawPartRate(string bmpfile, string section, int interval);

	void RunDrawRate();

	int GetRateFrequency();

	int GetRateDesXY(string lpAppName, string nametype, int x);

	bool BeforeRun();
	bool Do();
	bool AfterRun();

};

#endif /* DRAWRATE_H_ */
