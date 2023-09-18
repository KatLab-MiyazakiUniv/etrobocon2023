#!/bin/bash
<<DOCUMENT_COMMENT
@file   start_rear_camera.sh
@brief  リアカメラ用のサーバを起動する
@author 
DOCUMENT_COMMENT

cd /home/et2023/work/RasPike/sdk/workspace/etrobocon2023/rear_camera_py
python -m src $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}
