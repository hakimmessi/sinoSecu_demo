package com.ktk.sim_dispenser_demo_app;

import com.sun.jna.Native;
import com.sun.jna.Library;
import com.sun.jna.WString;
import com.sun.jna.ptr.IntByReference;

public interface CIDCardRecog extends Library {
    CIDCardRecog INSTANCE = Native.load("IDCard", CIDCardRecog.class);

    int InitIDCard(WString lpUserID, int nType, WString lpDirectory);
    int FreeIDCard();
    int DetectDocument();
    int AutoProcessIDCard(IntByReference nCardType);
}
