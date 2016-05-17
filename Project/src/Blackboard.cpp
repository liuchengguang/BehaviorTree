//All content © 2015 DigiPen(USA) Corporation, all rights reserved.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Luis Barrios
#include "Blackboard.h"
Blackboard::Blackboard( )
{
    m_parameters["NEO_dist"] = new float( );
    m_parameters["NEO_obj"] = NULL;
    m_parameters["WTP_posX"] = new float( );
    m_parameters["WTP_posZ"] = new float( );
    m_parameters["WAN_vel"] = new float( );
    m_parameters["WAN_angle"] = new float( );
    m_parameters["WAN_change"] = new float( );
    m_parameters["ESCPT_vel"] = new float( );
    m_parameters["ESCPT_angle"] = new float( );
    m_parameters["ESCPT_change"] = new float( );
    m_parameters["ESCPT_obj"] = NULL;
    m_parameters["ESCPT_dist"] = new float( );
    m_parameters["ESCPT_time"] = new float( );
    m_parameters["ESCPT_recover"] = new float( );
    m_parameters["HLWT_val"] = new float( );
    m_parameters["REPT_val"] = new float( );
    m_parameters["REP_val"] = new float( );
    m_parameters["MLE_vel"] = new float( );
    m_parameters["MLE_obj"] = NULL;
    m_parameters["MLE_dist"] = new float( );
    m_parameters["MLE_time"] = new float( );
    m_parameters["MLE_recover"] = new float( );
    m_parameters["CON_vel"] = new float( );
    m_parameters["CON_obj"] = NULL;
}

Blackboard::~Blackboard( )
{
    delete m_parameters["NEO_dist"];
    delete m_parameters["WTP_posX"];
    delete m_parameters["WTP_posZ"];
    delete m_parameters["WAN_vel"];
    delete m_parameters["WAN_angle"];
    delete m_parameters["WAN_change"];
    delete m_parameters["ESCPT_vel"];
    delete m_parameters["ESCPT_angle"];
    delete m_parameters["ESCPT_change"];
    delete m_parameters["ESCPT_dist"];
    delete m_parameters["ESCPT_time"];
    delete m_parameters["ESCPT_recover"];
    delete m_parameters["HLWT_val"];
    delete m_parameters["REPT_val"];
    delete m_parameters["REP_val"];
    delete m_parameters["MLE_vel"];
    delete m_parameters["MLE_dist"];
    delete m_parameters["MLE_time"];
    delete m_parameters["MLE_recover"];
    delete m_parameters["CON_vel"];
}

void Blackboard::SetParameterInt( std::string key, int value )
{
    *((int *)m_parameters[key]) = value;
}

void Blackboard::SetParameterFloat( std::string key, float value )
{
    *((float *)m_parameters[key]) = value;
}

void Blackboard::SetParameterGameObj( std::string key, GameObject *value )
{
    m_parameters[key] = value;
}

int Blackboard::GetParameterInt( std::string key )
{
    return *((int*)m_parameters[key]);
}

float Blackboard::GetParameterFloat( std::string key )
{
    return *((float*)m_parameters[key]);
}

GameObject* Blackboard::GetParameterGameObj( std::string key )
{
    return ((GameObject*)m_parameters[key]);
}