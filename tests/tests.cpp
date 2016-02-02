
#include "gtest/gtest.h"
#include "TestSuite.hpp"

// ------------- Tests for JTimer class ------------- //
TEST (JTimer, start)
{
    JTimer* t = new JTimer();
    t->start();
    
    EXPECT_EQ(t->isStarted(), true);
    EXPECT_TRUE(t->getTicks() == SDL_GetTicks());
}

TEST (JTimer, stop)
{
    JTimer* t = new JTimer();
    t->start();
    t->stop();
    
    EXPECT_EQ(t->isStarted(), false);
    EXPECT_TRUE(t->getTicks() == 0);
}

TEST (JTimer, pause)
{
    JTimer* t = new JTimer();
    t->start();
    t->pause();
    
    EXPECT_EQ(t->isPaused(), true);
    EXPECT_TRUE(t->getTicks() == 0);
}

TEST (JTimer, unpause)
{
    JTimer* t = new JTimer();
    t->start();
    Uint32 ticks = t->getTicks();
    t->pause();
    t->unpause();
    
    EXPECT_EQ(t->isPaused(), false);
    EXPECT_TRUE(t->getTicks() == ticks);
}

TEST (JTimer, getTicks)
{
    JTimer* t = new JTimer();
    EXPECT_EQ(t->getTicks(), 0);
    
    t->start();
    EXPECT_EQ(t->getTicks(), SDL_GetTicks());
    
    t->pause();
    EXPECT_TRUE(t->getTicks() == SDL_GetTicks());
}

// ------------- Tests for the Piece class ------------- //
TEST (Piece, setButton)
{
    Piece* p = new Piece();
    p->setButton(5, 6, 1);
    
    EXPECT_EQ(p->getPosition().x, 5);
    
    EXPECT_EQ(p->getPosition().y, 6);
    
    EXPECT_EQ(p->getPicNum(), 1);
}

TEST (Piece, setPicNum)
{
    Piece* p = new Piece();
    p->setPicNum(5);
    
    EXPECT_EQ(p->getPicNum(), 5);
}

TEST (Piece, setPosition)
{
    Piece* p = new Piece();
    p->setPosition(10,11);

    EXPECT_EQ(p->getPosition().x, 10);
    
    EXPECT_EQ(p->getPosition().y, 11);
}

TEST (Piece, handleEvent)
{
    Piece* p = new Piece();
    p->setPosition(10,11);
    
    SDL_Event* e;
    //e->type = SDL_MOUSEBUTTONDOWN;
    
    p->handleEvent(e);
    
    EXPECT_EQ(p->getPosition().x, 10);
    
    EXPECT_EQ(p->getPosition().y, 11);
}

// ------------- Tests for the JTexture class ------------- //
TEST (JTexture, loadFromFile)
{
    
    JTexture t;
    
    //t.loadFromFile("altchuler.bmp")
    
    EXPECT_TRUE(!t.loadFromFile("notHere.bmp"));
}

TEST (JTexture, loadFromRenderedText)
{
    JTexture t;
    std::stringstream s;
    s << "Hi";
    SDL_Color textColor = { 0, 0, 0, 255 };
    
    //EXPECT_TRUE(t.loadFromRenderedText(s.str().c_str(), textColor));
    
    std::string n;
    
    EXPECT_TRUE(!t.loadFromRenderedText(n, textColor));
}

TEST (JTexture, free)
{
    JTexture t;
    t.loadFromFile("altchuler.bmp");
    t.free();
    
    EXPECT_TRUE(t.getHeight() == 0);
    EXPECT_TRUE(t.getWidth() == 0);
}







int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

