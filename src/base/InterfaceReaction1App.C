#include "InterfaceReaction1App.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<InterfaceReaction1App>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

InterfaceReaction1App::InterfaceReaction1App(InputParameters parameters) : MooseApp(parameters)
{
  InterfaceReaction1App::registerAll(_factory, _action_factory, _syntax);
}

InterfaceReaction1App::~InterfaceReaction1App() {}

void
InterfaceReaction1App::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAll(f, af, s);
  Registry::registerObjectsTo(f, {"InterfaceReaction1App"});
  Registry::registerActionsTo(af, {"InterfaceReaction1App"});

  /* register custom execute flags, action syntax, etc. here */
}

void
InterfaceReaction1App::registerApps()
{
  registerApp(InterfaceReaction1App);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
InterfaceReaction1App__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  InterfaceReaction1App::registerAll(f, af, s);
}
extern "C" void
InterfaceReaction1App__registerApps()
{
  InterfaceReaction1App::registerApps();
}
