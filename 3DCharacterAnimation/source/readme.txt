TN Animator version 1.0
Release Notes
Tanzanite Software
David Paull

----------------------------------------------------------------------------------------------

This is the first release of TN Animator Software.
you are entitled to free Source Code updates to TN Animator Private 
if you are an owner of Charles River Media's 
"Programming Dynamic Character Animation" Book.

Please read SoftwareLicenseAgreement.doc Which is also on the CDROM

Tutorials, Updates, and Sample animation data can be found here :

http://www.tanzanite.to/

Warning1 : this has not been tested with voodoo2 hardware. Windowed mode will fail on voodoo2.
----------------------------------------------------------------------------------------------

QUICK START GUIDE :

	1) Copy the "sourcecode" directory from the books CD to your Hard drive(HD).
	2) Using windows explorer, find the new "Sourcecode" directory on your HD.
	3) right click on the "Sourcecode" directory, and select properties.
	4) remove the check mark next to the read only attribute.
	5) select ok, and then select, "Apply changes to this folder, subfolders, and files."  and select ok.
	6) Load sourcecode\Render.dsw into Visual C++ by double clicking on it.
	7) Select Build, and then ReBuild ALL.
	8) Hit F5 to run in debug mode. your windows desktop should be IN AT LEAST 1024x768
	9)Select windowed mode.
		-Demo.scn will load automatically.
		-this app is always in mouse look mode unless 
			-the shift, or ctrl key is down, allowing rotation, movement of objs.
			-or if in menu mode. TAB key toggles menu
	10)use mouse to center Girl on screen. 

		MOVE CAMERA 

		arrow keys, or W,A,S,Z move the camera.
		
		 W
		A S	   Forward, strafe left, strafe right, backward
		 Z

		ROTATE CAMERA using mouse look.
		TWIST CAMERA using mouse look, while holding right mouse button down.

		optional : You can use numerical keypad for keyboard rotation(like mouse).

	11)Hit F3 to enter edit bone mode.
	12)Hit 1 to set anim at 0
	13)Hit 5 to play the simplest anim ever created. 
		Anim was simply demonstrates interpolation, and movement.
		New Animations can be found at www.tanzanite.to

----------------------------------------------------------------------------------------------

This App can load these models
	1)Milkshape ASCII *.txt files using OpenObject()
		note : Milkshape can convert *.3ds into *.txt
	2)Internal Formats *.obj, *.msh, *.ske

----------------------------------------------------------------------------------------------


-note from author : 

This is release of version 1.3 The entire system has 32,280 lines of code. 
When the book is released, there will no doubt be some bug reports. 
All bugs will be fixed, and source code changes will be made available to owners of the book.
It is advised for you to get the latest version of the source code from the Internet today if possible.
Software updates are available only to owners of "Programming Dynamic Character Animation" Book.

A CD containing the latest version can be mailed to you.
Send $10 United States Dollars to 

David Paull 
PO BOX 1649 
Hanalei, HI, 96714.

please verify current mailing address by going to 
the website  	www.tanzanite.to 
or email 		etinaznat@hotmail.com

----------------------------------------------------------------------------------------------

Some design specs have not been completed yet.
All will be available as future release.

	1) !! When selecting a bone, you have to point at its base first. a bit tricky. will fix.
		a)The rotate sphere has a radius of 1 until a bone is selected.
			Then the radius is bone length.
		b) click on base of bone, and move mouse a little, and then the control sphere is correct.	

	2) Spraying/Editing weights is not very easy to use. Luckily InitWeights handles most of the work.
		a) If a vert is 100% to one bone, you cant reduce it. any reducion gets re-normalized to 100%
			1)instead of removing influence, add that vert to another bone.
			2)spra
		b) sometimes moving camera is required to get vertex to change color
		c) spraying goes through body. you can accidentally spray on arm through body.
		d) sometimes you cant add(spray green), choose other logical bone, and add. blending then possible
		e) sometimes you cant subtract(remove green), choose other logical bone, and subtract.

	3) All Buttons/Icons use function keys only. 
		a) they will be mouse clickable.

	4) No sample motion data on CD. This CD does not contain any animation data.
		a) demo.scn has a very small anim verifying anim abilities.

	5) On my system, Milkshape prevents this project from initializing Direct3d. 
		a) close milkshape prior to running TNAnimator.
		b) this may be caused by beta Radeon drivers.

	6) can only load .txt into Object, not Mesh.

	7) Its not obvious what keyframe you are on in Object mode.
		a) since the "object matrix" is really the rootnode bone matrix, keyframe is important.
		b) an actual key frame must be selected to move or rotate the Object. (each key has position)
	
	8) The matrix version of SLERP has a severe accuracy problem. ;(
		a) apparently the axis of rotation, or angle is not 100% accurate	
		b) as a result, R!=B where R=SLERP(R,A,B,1.0). it should ==B if weight is 1.0
		c) this results in a pop when switching to next keyframe.
		d) When this is fixed, I assume the matrix version will replace the now Quaternion version.

	9) ALT+TAB causes loss of textures in openGL fullscreen

	10) Version 1 is for readability. Future version will be optimized for speed.

	11) Edit Vertex has button, but is not implemented.

	12) Edit Triangle has button, but is not implemented.

	13) Move Bone has button, but is not implemented.

	14) Milkshape bones are not loaded.

	15) there is no scene clipboard as described in text. you can create a new scene, same thing.

	16) the girl models face has bad shading artifacts around mouth.
		a) special code was written to smooth the normals across multiple meshes.
		b) access to this code is through code only at moment
		c) The girl was loaded as Milkshape .txt using the following function:
			pBodyObject = SceneMenu_LoadTXT(pScene,"bookgirlbody3.txt",true);
		d) true signifies that you want smoothing groups activated during load.
		e) The smooth model was then saved into TN Animator's .obj format.


If you encounter a BUG, dont panic, it may already be fixed.
please email all bug reports, and suggestions for bug fixes to 

david@tanzanite.to
or
etinaznat@hotmail.com




----------------------------------------------------------------------------------------------
	

The Demo Girl was created using the following


	1) Girl.max was created in 3D Studio MAX by artists at www.ballisticpixel.com.
		a) http://www.ballisticpixel.com
	2) Girl.max was converted into Girl.3ds using 3D Studio MAX	
	3) Girl.3ds was imported into Milkshape modeler. Milkshape is Only $20. Well Worth It.
		a) http://www.swissquake.ch/chumbalum-soft/
	4) Using Milkshape, the model was rescaled, and orientated. Textures were re-loaded.
	5) The Milkshape model was saved as a milkshape file for later Milkshape editing.	
	6) The Milkshape model was exported as milkshape ASCII file *.txt .
	7) Girl.txt was loaded into an Object using TN Animator.
	8) A Skeleton was created from scratch.
		a) after many trial, and error tries, the skeleton creation boiled down to this:
		b) Initial angle is based on prev bone, and is confusing. this will change.

		NewBone( Name,Length,InitialAngle,ParentName) ; description

		------------------------------------------
		NewBone(	B0	9	0	Root	;Back Bone 1
		NewBone(	B1	10	0	B1		;Back Bone 2
		NewBone(	B2	5	0	B2		;Neck Bone 1
		**NewBone(	B3	5	0	B3		;Neck Bone 1
		------------------------------------------
		NewBone(	B4	7	250	B1		;Right Shoulder
		NewBone(	B5	8	10	B4		;Right Arm
		NewBone(	B6	9	10	B5		;Right Fore Arm
		------------------------------------------
		NewBone(	B7	7	110	B1		;Left Shoulder
		NewBone(	B8	8	350	B7		;Left Arm
		NewBone(	B9	9	350	B8		;Left Fore Arm
		------------------------------------------
		NewBone(	B10	5	225	Root	;Right Hip
		NewBone(	B11	15	310	B10		;Right Leg
		NewBone(	B12	20	5	B11		;Right Shin
		NewBone(	B13	5	0	B12		;Right Foot
		------------------------------------------
		NewBone(	B14	5	135	Root	;Left Hip
		NewBone(	B15	15	50	B14		;Left Leg
		NewBone(	B16	20	355	B15		;Left Shin
		NewBone(	B17	5	0	B16		;Left Foot
		------------------------------------------
		//warning, just prior to release, I changed the model to have legs farther apart
		//  for sake of InitWeights. 
		//  The above skeleton will need to be updated for B11, and B15 to accout for this
		//	if you plan on using that model.

		//** the second neck bone was deleted in demo1.scn. there is no longer a B3

	9) InitWeights() creates a best guess for bone weights. For this girl model, the bone weights are about 75% correct.
		Enter Edit Weights mode, and using F7 you can see the initial weights for mesh.
		Problems arise around legs, where code incorrectly chooses a the wrong bone.

		For Example, when B11 is selected, you can see that the other red bones also 
		contribute to the patch. B15 is other leg, so it shouldnt be a contributer.

		Using RemoveContributers(B15) while B11 is selected fixes that problem.


		------------------------------------------

	10) Frequent saving compensates for the lack of an undo feature. 
		Undo will be implemented in future release.

		------------------------------------------


	11) SkyBox was created using TerraGen

		http://www.planetside.co.uk/
		http://fps.brainerd.net/terragen.htm
----------------------------------------------------------------------------------------------
Trouble shooting :


0) 	c:\sourcecode\direct3ddll\direct3dtexturell.h(1) : 
	fatal error C1083: Cannot open include file: 'd3d8.h': No such file or directory

	answer) The following path must be included in Visual C++ Tools->Options->Directories : 
	C:\SOURCECODE\DIRECT3DDLL\INCLUDE


		goto Tools->options->Directories tab.
		add the following line, "c:\sourcecode\direct3ddll\include"

		make it the top most line, by dragging it with mouse.

		note: if you have DirectX 8 you should point to your DX dir instead like this:
		c:\DXSDK8\INCLUDE
		click "ok"


1) Debug Assertion Failed : Expression str!=NULL
	answer) The files must not be readonly. Placing files on a CDROM makes them readonly. 
		After copying the files to your HD, you must uncheck the read only attribute.
		See quick start guide (#1,2,3,4,5) above to remedy this.


2) The girl wont animate.
	answer) when TN Animator is first loaded, the default model is the skybox, 
			which has no animation data. Hit F1 to enter Object mode, and then F7 to select next Object(girl), 
			and then F3 to enter Edit Bone Mode.


3) I changed the Direct3D.cpp file, but nothing happens.
	answer) the rendering dlls are in seperate projects, and must be compiled.
	dont forget to compile dlls before running render.


4) My model does not follow the skeleton
	answer) Each Mesh in Object must be flagged as Transformed.
	answer) The Mesh in the restpose will not follow skeleton. This is where alignment of mesh, and skeleton occours.
	
5) My model deforms incorrectly when the skeleton moves.
	answer) this is totally controlled by vertex weights. weird things can happen if they are wrong.

6) I cant insert a Bone inbetween two existing Bones.
	answer) this has not been implemented yet. plan you skeleton carefully at begining.


7) all the sudden, the menu is gone.
	answer) sometimes when you select a different object, the closing of the previous object means that
			the menu is much shorter. The menu is there, it is simply off screen. Hit the home key to return to top.

8) The opengl version is running at 1/4 the speed as Direct3D. 
	answer) You probably have multimon enabled. OpenGL does not like multimon. For example id software's Quake3(opengl) wont run in multimon mode.
		disable multimon, and try again.
----------------------------------------------------------------------------------------------
BUGS:

1) Radeon card shows incorrect blend mode in OpenGL. Menu is unreadable.

----------------------------------------------------------------------------------------------


please email all bug reports, and bug fixes to 

david@tanzanite.to
or
etinaznat@hotmail.com


Thanks
David Paull


















