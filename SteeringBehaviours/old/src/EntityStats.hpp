#ifndef ENTITYSTATS_HPP
#define ENTITYSTATS_HPP

struct EntityStats
{
public:
    float       mass;
    float       maxSpeed;
    float       maxForce;
    float       maxTurnRate;

                EntityStats(float ma
                            , float speed
                            , float force
                            , float turnRate)
                : mass(ma)
                , maxSpeed(speed)
                , maxForce(force)
                , maxTurnRate(turnRate)
                {

                };
};

#endif // ENTITYSTATS_HPP
