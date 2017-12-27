package com.iutils.utils;

import org.junit.runner.RunWith;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

import static org.junit.Assert.*;

/**
 * Created by kevin on 2017/11/26.
 */
//@RunWith(PowerMockRunner.class)
//@PrepareForTest({StringUtil.class})
public class StringUtilTest {

    @org.junit.Before
    public void setUp() throws Exception {
        System.out.println("setUp run");
    }

    @org.junit.Test
    public void testIsEmpty() throws Exception {
        //PowerMockito.mockStatic(StringUtil.class);
        //PowerMockito.when(StringUtil.class,"isEmpty"," ").thenReturn(false);
        assertTrue(StringUtil.isEmpty(" "));
        assertTrue(StringUtil.isEmpty(""));
        assertTrue(StringUtil.isEmpty(null));
        assertFalse(StringUtil.isEmpty("test"));
        /*
        assertTrue(StringUtil.isEmpty(" "));
        PowerMockito.when(StringUtil.class,"isEmpty","").thenReturn(false);
        assertTrue(StringUtil.isEmpty(""));
        PowerMockito.when(StringUtil.class,"isEmpty",null).thenReturn(false);
        assertTrue(StringUtil.isEmpty(null));
        PowerMockito.when(StringUtil.class,"isEmpty","test").thenReturn(true);
        assertFalse(StringUtil.isEmpty("test"));
        */
    }
}