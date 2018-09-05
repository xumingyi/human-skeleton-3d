#ifndef NAMES_H
#define NAMES_H

// Names of the bones and joints for the human skeleton

// Limbs 四肢
#define TORSO       "Torso"			//躯干
#define NECK        "Neck"			//脖子
#define HEAD        "Head"			//头	
#define RUPPERARMZ  "RUpperArmZ"  // Zero length link
#define RUPPERARMY  "RUpperArmY"  // Zero length link
#define RUPPERARMX  "RUpperArmX"  //右上手臂坐标
#define RLOWERARM   "RLowerArm"   //右下手臂
#define LUPPERARMZ  "LUpperArmZ"  // Zero length link
#define LUPPERARMY  "LUpperArmY"  // Zero length link
#define LUPPERARMX  "LUpperArmX"  //左上手臂坐标
#define LLOWERARM   "LLowerArm"   //左下手臂
#define RUPPERLEGZ  "RUpperLegZ"  // Zero length link
#define RUPPERLEGY  "RUpperLegY"  // Zero length link
#define RUPPERLEGX  "RUpperLegX"  //右上腿坐标
#define RLOWERLEG   "RLowerLeg"   //右下腿
#define RFOOT       "RFoot"       //右脚
#define LUPPERLEGZ  "LUpperLegZ"  // Zero length link
#define LUPPERLEGY  "LUpperLegY"  // Zero length link
#define LUPPERLEGX  "LUpperLegX"  //左上腿坐标
#define LLOWERLEG   "LLowerLeg"   //左下腿
#define LFOOT       "LFoot"       //左脚

// Joints 关节点
#define ROOT        "Root"               //root基点 静止时躯干底部中心点，动的时候左右脚分别为基点。
#define NECKBASE    "NeckBase"           //脖子
#define HEADBASE    "HeadBase"			 //头
#define RSHOULDERZ  "RShoulderZ"		 //右肩膀Z
#define RSHOULDERY  "RShoulderY"		 //右肩膀Y
#define RSHOULDERX  "RShoulderX"		 //右肩膀X
#define RELBOW      "RElbow"             //右肘部
#define LSHOULDERZ  "LShoulderZ"         //左肩膀Z
#define LSHOULDERY  "LShoulderY"         //左肩膀Y
#define LSHOULDERX  "LShoulderX"	     //左肩膀X
#define LELBOW      "LElbow"             //左肘部

#define RHIPZ       "RHipZ"             //右臀部Z	
#define RHIPY       "RHipY"             //右臀部Y	
#define RHIPX       "RHipX"			    //右臀部X	
#define RKNEE       "RKnee"             //右膝盖
#define RANKLE      "RAnkle"            //右踝关节
#define LHIPZ       "LHipZ"			    //左臀部Z
#define LHIPY       "LHipY"				//左臀部Y
#define LHIPX       "LHipX"				//左臀部X	
#define LKNEE       "LKnee"				//左膝盖
#define LANKLE      "LAnkle"			//左踝关节

#endif
