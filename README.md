# CustomWidgetProj
**Use Slate and UMG to impletement flipbook animation**

I have searched the solution of UI flipbook animation for a long time, but there is none provides full control of the animation,like pause and stop.For example,the flipbook node in material editor is hard to pause or just play once.So I decide to find another way to do that.Although this should not be the best way to do that, I am closer to my purpose. 

Features:
+ Provide simple control of the animation, like playing, pausing and so on.
+ Implement the main logic in slate, and wrap it with UWidget, so it is easy to use in UMG.
+ Just implement an OnAnimationEnd delegate, you can use it like OnClicked of the button. 


####Important:
> UE4 version: 4.15.1
