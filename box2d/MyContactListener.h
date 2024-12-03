#pragma once
#include <vector>
#include "Ball.h"
#include "Collectible.h"
#include "Flag.h"
#include "Water.h"
class MyContactListener : public b2ContactListener {
public:
    MyContactListener(Ball& ball, std::vector<Collectible*>& collectibles, std::vector<Collectible*>& toRemove, bool& jumpFlag, Water& water, Flag& flag)
        : ball(ball), collectibles(collectibles), toRemove(toRemove), isJumping(jumpFlag), water(water), flag(flag) {
    }

    void BeginContact(b2Contact* contact) override {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        // Check for ground contact
        if (bodyA == ball.getBody() || bodyB == ball.getBody()) {
            if (bodyA->GetType() == b2_staticBody || bodyB->GetType() == b2_staticBody) {
                isJumping = false;
            }
        }

        // Ball collides with water
        if ((bodyA == ball.getBody() && bodyB == water.getBody()) ||
            (bodyB == ball.getBody() && bodyA == water.getBody())) {
            printf("Ball collided with water!\n");
            if (ball.isMaximized) {
                ball.startWaveEffect(); // Trigger wave effect
                ballGravity = ball.getBody()->GetGravityScale();
                ball.getBody()->SetGravityScale(0.0);

            }
            water.startWaveEffect(); // Trigger wave effect
        }

        // Ball collides with flag
        if ((bodyA == ball.getBody() && bodyB == flag.getBody()) ||
            (bodyB == ball.getBody() && bodyA == flag.getBody())) {
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            printf("Winner!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            flag.onCollision();

        }

        // Check for collectible collision
        for (auto& collectible : collectibles) {
            if (bodyA == collectible->getBody() || bodyB == collectible->getBody()) {
                toRemove.push_back(collectible); // Mark collectible for removal
            }
        }
    }

    void EndContact(b2Contact* contact) override {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        // Ball exits water
        if ((bodyA == ball.getBody() && bodyB == water.getBody()) ||
            (bodyB == ball.getBody() && bodyA == water.getBody())) {
            printf("Ball exited the water!\n");
            ball.getBody()->SetGravityScale(ballGravity);
        }
    }

private:
    Ball& ball;
    std::vector<Collectible*>& collectibles;
    std::vector<Collectible*>& toRemove;
    bool& isJumping;
    Water& water;
    Flag& flag;
    float ballGravity;
};  