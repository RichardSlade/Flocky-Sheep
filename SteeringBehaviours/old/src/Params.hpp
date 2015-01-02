#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <fstream>
#include <string>
#include <stdexcept>

#include <SFML/Graphics/Color.hpp>

struct Params
{
public:
    Params(std::string fileName)
    {
        std::ifstream in(fileName);

        if(!in.is_open())
            throw std::runtime_error("Error loading: " + fileName);

        in >> *this;

        in.close();
    };

    int LevelBlockSize;
    int WorldX;
    int WorldY;
    int NumSheep;

    float WindowX;
    float WindowY;
    float WaypointRadius;
    float ScrollSpeed;
    float DogMaxRotation;
    float SightRange;
    float AngleOfVision;
    float PanicDistance;
    float SheepMaxRotation;
    float PI;
    float WanderRadius;
    float WanderDistance;
    float WanderJitter;
    float InteractionRadius;
    float FeelerLength;
    float MinArriveDist;
    float ObstacleAvoidanceMultiplier;
    float WallAvoidanceMultiplier;
    float ArriveMultiplier;
    float EvadeMultiplier;
    float WanderMultiplier;
    float SeperationMultiplier;
    float AlignmentMultiplier;
    float CohesionMultiplier;
    float FlockingMultiplier;
    float MinViewBoxLength;

    friend void operator>>(std::ifstream &in, Params &params)
    {
        in.ignore(255, ' ');
        in >> params.LevelBlockSize;

        in.ignore(255, ' ');
        in >> params.WorldX;

        in.ignore(255, ' ');
        in >> params.WorldY;

        in.ignore(255, ' ');
        in >> params.NumSheep;

        in.ignore(255, ' ');
        in >> params.WindowX;

        in.ignore(255, ' ');
        in >> params.WindowY;

        in.ignore(255, ' ');
        in >> params.WaypointRadius;

        in.ignore(255, ' ');
        in >> params.ScrollSpeed;

        in.ignore(255, ' ');
        in >> params.DogMaxRotation;

        in.ignore(255, ' ');
        in >> params.SightRange;

        in.ignore(255, ' ');
        in >> params.AngleOfVision;

        in.ignore(255, ' ');
        in >> params.PanicDistance;

        in.ignore(255, ' ');
        in >> params.SheepMaxRotation;

        in.ignore(255, ' ');
        in >> params.PI;

        in.ignore(255, ' ');
        in >> params.WanderRadius;

        in.ignore(255, ' ');
        in >> params.WanderDistance;

        in.ignore(255, ' ');
        in >> params.WanderJitter;

        in.ignore(255, ' ');
        in >> params.InteractionRadius;

        in.ignore(255, ' ');
        in >> params.FeelerLength;

        in.ignore(255, ' ');
        in >> params.MinArriveDist;

        in.ignore(255, ' ');
        in >> params.ObstacleAvoidanceMultiplier;

        in.ignore(255, ' ');
        in >> params.WallAvoidanceMultiplier;

        in.ignore(255, ' ');
        in >> params.ArriveMultiplier;

        in.ignore(255, ' ');
        in >> params.EvadeMultiplier;

        in.ignore(255, ' ');
        in >> params.WanderMultiplier;

        in.ignore(255, ' ');
        in >> params.SeperationMultiplier;

        in.ignore(255, ' ');
        in >> params.AlignmentMultiplier;

        in.ignore(255, ' ');
        in >> params.CohesionMultiplier;

        in.ignore(255, ' ');
        in >> params.FlockingMultiplier;

        in.ignore(255, ' ');
        in >> params.MinViewBoxLength;
    }

};

#endif // PARAMS_HPP
