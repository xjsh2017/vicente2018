
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

NSViews C++ Library Version 1.04

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MFC Developers:

All of the source code for the NanoSoft NSViews C++ Library is yours free of 
charge in accordance with the license terms found in LICENSE.txt courtesy 
of NanoSoft Corporation.

Try it for you MFC app. The resizable dialogs, property sheets, and views
get addictive. We think that you and your users will like them. Feel free
to contact us:


         Michael A. Knewtson (Mike)

             Principal Engineering Consultant
             NanoSoft Corporation
             e-mail: knewtson@nanocorp.com or
                     knewtson@neosoft.com

         Edward C. Smetak (Ed)

             Vice President of Engineering Software Consulting
             NanoSoft Corporation
             e-mail: ecsmetak@nanocorp.com or 
                     ecsmetak@neosoft.com

         NanoSoft Corporation

             3040 Post Oak Blvd., Suite 1430
             Houston, Texas  77056
             (713) 961-3061 - Fax 961-4028
             http://www.nanocorp.com 
             e-mail: info@nanocorp.com  


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

NSViews C++ Library Release Notes

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ver 1.00 - 9/18/96

  Original code.

Ver 1.01 - 9/26/96

  Minor changes to source code comments.

Ver 1.02 - 10/9/96

  1. Fixed spelling error in CNSFlexConstraintList::PositionControls.
  2. Use TRACE instead of ASSERT for checking that a CNSFlexMDIChildWnd parent
      frame is used with our views.
  3. Don't size the CNSFlexPropertySheet smaller than the scroll size in
      a CNSFlexPropertySheetView.
  4. CNSFlexChildWnd is now DECLARE_DYNCREATE instead of DECLARE_DYNAMIC
      so that it can be used directly.

Ver 1.03 - 5/4/97 (NSViews WORKS FOR SDI APPLICATIONS TOO)

  A fair number of people who have downloaded NSViews have asked us whether
  it works for SDI applications. Although we didn't think about SDI apps
  when we designed the code, it turns out that Ver 1.03 works just fine!

  Revised CNSFlexPropertyPage::GetDocument() and CNSFlexPropertyPage::GetView
  to make them work properly for SDI applications. Note that CNSFlexFormView,
  CNSFlexPropertySheet, and CNSFlexPropertyPage will work just fine in the
  main window of SDI applications! The only fly in the ointment is setting
  the minimum size of the application's main window based on the size of the
  form view or property pages. Since these sizing calculations are based on
  whether the status and tool bars are visible, docked, etc. it turned out 
  to be a lot of work to do the minimum sizing in NSViews. (i.e. it's free
  code and we don't have time to work out all the details) However, there's
  an easy hack that gets around this problem -- just handle the 
  ON_WM_GETMINMAXINFO message in your frame window and set the minimum size 
  to anything you want. Also, you need to add handlers for the menu commands 
  ID_VIEW_STATUS_BAR and ID_VIEW_TOOLBAR to force an Invalidate when the
  toolbar or status bar is toggled on or off.  See CMainFrame::OnGetMinMax-
  Info(), CMainFrame::OnViewStatusBar(), and CMainFrame::OnViewToolbar() in
  any of the SDI sample applications to take care of these details.

Ver 1.04 - 5/4/97 (NSViews WORKS WITH ActiveX CONTROLS)

  We learned that ActiveX controls were not getting properly repositioned -
  so we fixed things so they now work just fine. In addition, there were a
  couple more things to take care of to make SDI apps work.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEMO PROGRAMS INCLUDED WITH SOURCE CODE:

(1) NSViewsMDIDemo - Multiple Document Interface demo using flexible
    dialogs, flexible property sheets, flexible form views, and flexible
	property sheet views. This sample demonstrates a little of everything.

(2) NSViewsSDIDemo1 - Single Document Interface demo using a flexible
    property sheet for the main window.

(3) NSViewsSDIDemo2 - Single Document Interface demo using a flexible
    form view for the main window.

(4) NSViewsSDIDemo3 - Same as (2) with the addition of an ActiveX control.
    Note: This sample requires the Calendar Control 8.0 that ships with 
	Visual C++.

(5) NSViewsSDIDemo4 - Same as (3) with the addition of an ActiveX control.
    Note: This sample requires the Calendar Control 8.0 that ships with 
	Visual C++.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TAKE A LOOK AT OUR ARTICLE DESCRIBING AND DOCUMENTING NSViews:

    Dialogisthenics
	by Ed Smetak and Mike Knewtson
	Windows Tech Journal, February 1997, pp 20-26

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As of April 1997, nearly 2,000 copies of NSViews Ver 1.02 have been 
downloaded from the NanoSoft web site. We hope Ver 1.04 will keep you 
bending over backward to make your applications easier to use!

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
