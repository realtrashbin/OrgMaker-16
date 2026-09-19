
void ClickProcL(WPARAM wParam=NULL, LPARAM lParam=NULL); //Left MouseButton clicked
void ClickProcR(WPARAM wParam=NULL, LPARAM lParam=NULL); //Right MouseButton clicked
void ClickProcM(WPARAM wParam=NULL, LPARAM lParam=NULL); //Click scrolly wheel
void ClearDrag(); //When you stop dragging?
void MouseDrag(WPARAM wParam, LPARAM lParam); //Moving your mouse while a button is down.
void LButtonUP(WPARAM wParam, LPARAM lParam); //Left MouseButton up
void RButtonUP(WPARAM wParam, LPARAM lParam); //Right MouseButton up
void SelectAll(int FullTrack); //I think this makes the tracks glow
void SelectReset(); //No clue

