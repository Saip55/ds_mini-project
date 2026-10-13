#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include "core/BrowserHistory.hpp"
#include "data_structures/Exceptions.hpp"

class TestBrowser {
public:
    static void runAll() {
        testInitialState();
        testSingleVisit();
        testMultipleVisits();
        testBackNavigation();
        testForwardNavigation();
        testForwardInvalidationRule();
        testStateTransitionMatrix();
        testBoundaryExceptions();
        testAuditLog();
    }

private:
    static void testInitialState() {
        BrowserHistory b;
        auto state = b.getState();
        assert(!state.currentPage.has_value());
        assert(state.backStackItems.empty());
        assert(state.forwardStackItems.empty());
        assert(!b.canGoBack());
        assert(!b.canGoForward());
    }

    static void testSingleVisit() {
        BrowserHistory b;
        Page p = b.visit("example.com");
        assert(p.url == "https://example.com");
        assert(b.getCurrentPage().has_value());
        assert(b.getCurrentPage()->url == "https://example.com");
        assert(!b.canGoBack());
        assert(!b.canGoForward());
    }

    static void testMultipleVisits() {
        BrowserHistory b;
        b.visit("a.com");
        b.visit("b.com");
        b.visit("c.com");

        auto state = b.getState();
        assert(state.currentPage.has_value());
        assert(state.currentPage->url == "https://c.com");
        assert(state.backStackItems.size() == 2);
        assert(state.backStackItems[0].url == "https://a.com");
        assert(state.backStackItems[1].url == "https://b.com");
        assert(state.forwardStackItems.empty());
        assert(b.canGoBack());
        assert(!b.canGoForward());
    }

    static void testBackNavigation() {
        BrowserHistory b;
        b.visit("a.com");
        b.visit("b.com");
        b.visit("c.com");

        Page backPage = b.back();
        assert(backPage.url == "https://b.com");

        auto state = b.getState();
        assert(state.currentPage->url == "https://b.com");
        assert(state.backStackItems.size() == 1);
        assert(state.backStackItems[0].url == "https://a.com");
        assert(state.forwardStackItems.size() == 1);
        assert(state.forwardStackItems[0].url == "https://c.com");

        Page secondBack = b.back();
        assert(secondBack.url == "https://a.com");
        state = b.getState();
        assert(state.backStackItems.empty());
        assert(state.forwardStackItems.size() == 2);
        assert(state.forwardStackItems[0].url == "https://c.com");
        assert(state.forwardStackItems[1].url == "https://b.com");
    }

    static void testForwardNavigation() {
        BrowserHistory b;
        b.visit("a.com");
        b.visit("b.com");
        b.back(); // At a.com, forward has b.com

        Page fwdPage = b.forward();
        assert(fwdPage.url == "https://b.com");

        auto state = b.getState();
        assert(state.currentPage->url == "https://b.com");
        assert(state.backStackItems.size() == 1);
        assert(state.backStackItems[0].url == "https://a.com");
        assert(state.forwardStackItems.empty());
    }

    static void testForwardInvalidationRule() {
        BrowserHistory b;
        b.visit("a.com");
        b.visit("b.com");
        b.visit("c.com");

        b.back(); // At b.com, forward has c.com
        assert(b.canGoForward());

        // Visiting brand-new URL D must purge forward stack
        b.visit("d.com");
        auto state = b.getState();
        assert(state.currentPage->url == "https://d.com");
        assert(state.forwardStackItems.empty());
        assert(!b.canGoForward());
    }

    static void testStateTransitionMatrix() {
        BrowserHistory b;

        // Transition 1: [] -> VISIT A -> back=[], current=A, fwd=[]
        b.visit("a.com");
        auto s1 = b.getState();
        assert(s1.backStackItems.empty());
        assert(s1.currentPage->url == "https://a.com");
        assert(s1.forwardStackItems.empty());

        // Transition 2: [A], B, [] -> VISIT C -> back=[A, B], current=C, fwd=[]
        b.visit("b.com");
        b.visit("c.com");
        auto s2 = b.getState();
        assert(s2.backStackItems.size() == 2);
        assert(s2.backStackItems[0].url == "https://a.com");
        assert(s2.backStackItems[1].url == "https://b.com");
        assert(s2.currentPage->url == "https://c.com");
        assert(s2.forwardStackItems.empty());

        // Transition 3: From ([A], B, [C]), execute BACK -> back=[], current=A, fwd=[C, B]
        b.back(); // Now at B, back=[A], fwd=[C]
        b.back(); // Now at A, back=[], fwd=[C, B]
        auto s3 = b.getState();
        assert(s3.backStackItems.empty());
        assert(s3.currentPage->url == "https://a.com");
        assert(s3.forwardStackItems.size() == 2);

        // Transition 4: Reset to ([A], B, [C]), execute FORWARD -> back=[A, B], current=C, fwd=[]
        b.forward(); // Now at B, back=[A], fwd=[C]
        b.forward(); // Now at C, back=[A, B], fwd=[]
        auto s4 = b.getState();
        assert(s4.backStackItems.size() == 2);
        assert(s4.currentPage->url == "https://c.com");
        assert(s4.forwardStackItems.empty());
    }

    static void testBoundaryExceptions() {
        BrowserHistory b;
        bool caughtBack = false;
        try {
            b.back();
        } catch (const NavigationException&) {
            caughtBack = true;
        }
        assert(caughtBack);

        b.visit("single.com");
        caughtBack = false;
        try {
            b.back();
        } catch (const NavigationException&) {
            caughtBack = true;
        }
        assert(caughtBack);

        bool caughtFwd = false;
        try {
            b.forward();
        } catch (const NavigationException&) {
            caughtFwd = true;
        }
        assert(caughtFwd);
    }

    static void testAuditLog() {
        BrowserHistory b;
        b.visit("site1.com");
        b.visit("site2.com");
        b.visit("site3.com");
        auto log = b.getAuditLog();
        assert(log.size() == 3);
        assert(log[0].url == "https://site1.com");
        assert(log[1].url == "https://site2.com");
        assert(log[2].url == "https://site3.com");
    }
};
