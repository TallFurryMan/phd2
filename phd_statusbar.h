/*
*  phd_statusbar.h
*  PHD Guiding
*
*  Created by Bruce Waddington
*  Copyright (c) 2016 Bruce Waddington
*  All rights reserved.
*
*  This source code is distributed under the following "BSD" license
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*    Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*    Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*    Neither the name of Bret McKee, Dad Dog Development,
*     Craig Stark, Stark Labs nor the names of its
*     contributors may be used to endorse or promote products derived from
*     this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
*  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
*  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
*  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef _PHD_STATUSBAR_H_
#define _PHD_STATUSBAR_H_

// Types of fields in the statusbar
enum SBFieldTypes
{
    Field_Msg1,
    //Field_Mass,
    Field_Sat,
    Field_SNR,
    //Field_RADir,
    //Field_RAAmount,
    Field_RAInfo,
    //Field_DecDir,
    //Field_DownArrow,
    Field_DecInfo,
    Field_Darks,
    Field_Calib,
    Field_Gear,
    //Field_Mount,
    //Field_AO,
    //Field_Rot,
    Field_Max
};

class PHDStatusBar;

// Class for color-coded state indicators
class SBStateIndicatorItem
{
public:
    SBFieldTypes type;
    int txtHeight;
    int txtWidth;
    int fieldId;
    int lastState;
    PHDStatusBar* parentSB;
    wxStaticText* ctrl;
    wxStaticBitmap* pic;
    wxString otherInfo;

public:
    SBStateIndicatorItem(PHDStatusBar* parent, int indField, const wxString &indLabel, SBFieldTypes indType, std::vector <int> &fldWidths);
    void PositionControl();
    void UpdateState();
    wxString IndicatorToolTip(SBFieldTypes indType, int triState);
};

class SBStateIndicators
{
    SBStateIndicatorItem* stateItems[Field_Max - Field_Darks];
    int numItems = Field_Max - Field_Darks;
    PHDStatusBar* parentSB;


public:
    SBStateIndicators(PHDStatusBar* parent, std::vector <int> &fldWidths);
    ~SBStateIndicators();
    void PositionControls();
    void UpdateState();

};

class SBGuideIndicators
{
    wxStaticBitmap* bitmapRA;
    wxStaticBitmap* bitmapDec;
    wxStaticText* txtRaAmounts;
    wxStaticText* txtDecAmounts;
    wxIcon icoLeft;
    wxIcon icoRight;
    wxIcon icoUp;
    wxIcon icoDown;
    PHDStatusBar* parentSB;

public:
    SBGuideIndicators(PHDStatusBar* parent, std::vector <int> &fldWidths);
    void PositionControls();
    void UpdateState(GUIDE_DIRECTION raDirection, GUIDE_DIRECTION decDirection, double raPx, double raPulse, double decPx, double decPulse);
    void ClearState() { UpdateState(LEFT, UP, 0, 0, 0, 0); }

};

class SBStarIndicators
{
    wxStaticText* txtMassPct;
    wxStaticText* txtSNR;
    wxStaticText* txtSaturated;
    const wxString massStr = _("Mass");
    const wxString SNRStr = _("SNR");
    const wxString satStr = _("SAT");
    int massWidth;
    int snrWidth;
    int satWidth;
    int txtHeight;
    PHDStatusBar* parentSB;

public:
    SBStarIndicators(PHDStatusBar *parent, std::vector <int> &fldWidths);
    void PositionControls();
    void UpdateState(double MassPct, double SNR, bool Saturated);

};

// Child of normal status bar - used for status bar with color-coded messages and state indicators
class PHDStatusBar : public wxStatusBar
{
public:
    PHDStatusBar(wxWindow *parent, long style = wxSTB_DEFAULT_STYLE);
    virtual ~PHDStatusBar();

    wxIcon yellowLight;
    wxIcon redLight;
    wxIcon greenLight;
    wxPoint FieldLoc(int fieldNum, int crtlWidth, int ctrlHeight);
    void SetStatusText(const wxString &text, int number = 0);
    void UpdateStates();
    void UpdateStarInfo(double SNR, bool Saturated);
    void ClearStarInfo() { UpdateStarInfo(-1, 0); }
    void UpdateGuiderInfo(GUIDE_DIRECTION raDirection, GUIDE_DIRECTION decDirection, double raPx, double raPulse, double decPx, double decPulse);
    void ClearGuiderInfo();

    // event handlers
    void OnSize(wxSizeEvent& event);

private:
    SBStateIndicators* m_StateIndicators;
    SBStarIndicators* m_StarIndicators;
    SBGuideIndicators* m_GuideIndicators;
    wxStaticText* m_Msg1;
    int m_NumIndicators;

    wxDECLARE_EVENT_TABLE();
};
#endif      // PHD_STATUS_H