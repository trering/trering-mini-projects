// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"


/**
* learnt:
	- download the example content project (example models, etc.). It has a 4.4GB install size.
		Go to the launcher, click on the
		"learn" tab. Click on "Example Content" and download on the top right. Once downloaded,
		in the udk launcher, click on library. "Content Examples" should now exist at the bottom.
		Click on "Create Project". Now you can open that up and look at how they do stuff.
	- can't remove components in the editor that are set up in code
	- "BlueprintNativeEvent" == can override in a blueprint
	- if something is a "BlueprintNativeEvent", then you have to add
		"_Implementation" in the definition of the function
	- "GENERATED_UCLASS_BODY" may be used instead of "GENERATED_BODY"
		so that you don't have to define the constructor
	- "Destroy()" calls destructor
	- two types of key bindings:
		access mapping == 1 keypress, something happens, finished (e.g. press space to jump)
		axis mapping == continuous blend (e.g. camera controls)
	- "InputComponent->BindAction("collectPickups", IE_Pressed, this, &ACharacter::collectBatteries);" in the character class
		says that when the "collectPickups" action happens, the ACharacter::collectBatteries() is called. The "collectPickups"
		action is defined in the UDK4 editor by going to:
		Edit --> Project Settings --> Engine (header) --> Inputs
		Then you can add a new mapping (a.k.a. key binding in normal english). The mapping created in this tutorial was called
		"collectPickups".
	- the easiest way to create a class is to use the wizard in UDK4. Go to:
		File --> Add code...
		This sorts out putting the files in the correct locations and adding the correct headers
	- "Cast<TypeToCastTo>(objectYouWantToCast)" looks to be like a dynamic cast (runtime cast that returns null if it couldn't
		cast).
	- when creating a "timeline" in the udk editor, you can create a constant so that you don't have to
		edit the blueprint every time. In this project I created a "MyGameData" project in the content browser
		(in the udk editor) and right clicked in the empty space inside the folder (inside the content browser),
		then clicked on misc, then on curve. I created a "floating point curve". I opened up the curve and
		double clicked on two random points. I selected the first point and set the time and value to 0. I
		selected the second point and set the time to 1.5 and the value to 1. Then in the timeline (whilst
		having the curve as the last thing selected in the content browser) I created a new function (button
		that has an "f" on it) and then clicked on the backspace arrow to insert the curve I made. I then
		clicked on "use last keyframe" to "stop it happening every 5 seconds"(?).
	- (in the udk editor) holding the alt key while dragging a variable onto the event graph will give you a
		setter for that variable.
	- ^^ holding down control gives you a getter
	- you can copy things from one udk project to another. Go into the udk editor and find the thing that you
		want in the content explorer. Right click --> asset actions --> migrate. Click "OK" on the next window
		that appears (useless windows telling you the files that you're copying). Select the location of the
		project that you want to copy it to. It is not a good idea to rename the thing that you're copying
		otherwise you will have to import (in the content window) the thing separately and have to match up
		the mesh with the material afterwards.
	- we want that sphere that battery collision sphere that the player has to generate an "overlap" event
		when it collides with one of the batteries. We want the Battery to collide with everything else. So
		on the blueprint for the battery (BatteryPickupBlueprint in the udk editor) we go to the "components"
		tab. CLick on "mesh" and go to "collision" in the "details" pane. Note that the "collision preset" is
		set to "custom", the "object type" is "world dynamic" and under "object responses" the "world dynamic"
		option is set to "overlap" (all of the other options should be set to "block").
	- it is the static mesh that deals with the collision stuff. If you want to add collision to a mesh then
		open the mesh via the content browser (I actually did this via the batterypickup blueprint, double
		clicking the mesh component's static mesh picture). Then click on the "collision" menu and the more
		simple meshes are at the top and the complex ones at the bottom. You can add multiple collision
		geometries (e.g. a tightly fitting collision geometry sitting inside a sphere collision geometry).
	- how to access the player (and its variables) from a blueprint. Right click on blueprint
		and search for "GetPlayerCharacter". Click and drag from the little circle next to "return value" and
		a box will appear, search for "cast to <NAMEOFCLASSTHATYOUCALLEDYOURCHARACTERINC++CODE>". Now you can
		access variables and member functions off him. There is also a generic "MyCharacter" class that you
		inherit from (I think) and you can just cast it to this but you'll only be able to get the blueprint
		stuff that is attached to the character from that MyCharacter class.
	- with a particle animation (a "particle system" in udk speak) you need to change the "source" to "emitter"
		and "target" to "user defined". See the box in the middle labelled "emitter", source and target are
		there. Go to "source method" or "target method" (depends which one you have selected, they're the top
		items in the "details" pane) and change them. This is so that the source is the origin of where the
		particle animation is created and starts from, which will be the battery. The target can then be set
		to whereever you want (probably going to the the player for this example project).
	= the "pickup" function that we made for the battery class needs to call its parent in the blueprint, so
		that we don't lose the default functionality of the pickup function. In the BatterPickupBlueprint, we
		right click and search for "pickup". You can then right click on the pickup function in the blueprint
		and click on "add call to parent function".
	- the event "pickup" on the BatteryPickupBlueprint (in the udk editor) has a "spawn emitter attached"
		function off it. This creates a particle animation (a "particle system" in udk speak) that will be
		created when whatever calls the thing that you attach it to (in this case the pickup event). You need
		to also tell it what mesh it should attach itself to, in this case we've used the battery mesh (which
		we got by clicking that tiny box labelled "show inherited variables" and dragging that over to
		"attach to component" on the "spawn emitter attached" function.
	- the place at which particle animations are spawned will (you can create a "manual offset" apparently)
		be the origin of the mesh (so where x, y and z are zero. i.e. where the red blue and green arrows
		touch if you have the mesh open in udk). In udk speak they call this the "pivot" (why epic? why?!).
	- note that "location type" is "snap to target" on the "spawn emitter attached" function. The tutorial
		says "This makes it go to that pivot point", by which I think that they mean that it will stay at the
		origin, as one of the other options is to use an offset ("keep relative offset").
	- (in blueprint, udk editor) if you want to know the location of a bone or a socket, then it is best to
		get the location of the socket, because it is more generic and can be used for both.
	- (in blueprint, udk editor) if you want to know the name of a bone then go to the content browser and
		select the "Character" folder. One of the items displayed should be "HeroTTP" for the default
		character. Click on "skeleton" in the top right. The list of bones are on the left. The easiest way
		is to click on the button labelled "show" inside the window that displays a preview. Choose "bones"
		and "bone names". Note the spelling here, which will need to match to where you use it; like in the
		"get socket" function.
	- if you then want to add the item that you have created to the game you need to take the *BLUEPRINT*
		(in our case the "BatteryPickupBlueprint") and drag it into the game preview window.
	- you can add blueprint values to a 'watch' by right clicking them and selecting "watch this value". 
		Then you can see the value change as you run the game.
	- you can debug on different objects from within a blueprint (didn't use, just saw in video). If you're 
		looking a a blueprint, then there is a drop down box at the top where you can select the thing that 
		you want to debug on.
	- how to do testing via blueprints : https://www.youtube.com/watch?v=T7Yu7c6dYtI . Basically: make a 
		blueprint, and then call spawn actor and set the transform to be getactortransform. Also, add a 
		"billboard" component to the spawner so that it appears in the editor by going to components, add
		component and search for billboard.


	https://www.youtube.com/watch?v=sp5vd1AEDBY


*/

/**
 * dcr9 - apparently you can put tooltip text here
*/
UCLASS()
class NZQ7HGMXLP8_API APickup : public AActor
{
	GENERATED_BODY()

public:

	APickup(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAbleToBePickedUp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* baseCollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* mesh;

	UFUNCTION(BlueprintNativeEvent)
	void pickUp();

};


