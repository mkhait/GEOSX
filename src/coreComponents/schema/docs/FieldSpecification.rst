

====================== ============ ======== ============================================================== 
Name                   Type         Default  Description                                                    
====================== ============ ======== ============================================================== 
bcApplicationTableName string                Name of table that specifies the on/off application of the bc. 
beginTime              real64       -1e+99   time at which BC will start being applied.                     
component              integer      -1       Component of field (if tensor) to apply boundary condition to  
direction              R1Tensor     {0,0,0}  Direction to apply boundary condition to                       
endTime                real64       1e+99    time at which bc will stop being applied                       
fieldName              string                Name of field that boundary condition is applied to.           
functionName           string                Name of function that specifies variation of the BC            
initialCondition       integer      0        BC is applied as an initial condition.                         
name                   string       required A name is required for any non-unique nodes                    
objectPath             string                Path to the target field                                       
scale                  real64       0        Scale factor for value of BC.                                  
setNames               string_array required Name of sets that boundary condition is applied to.            
====================== ============ ======== ============================================================== 


