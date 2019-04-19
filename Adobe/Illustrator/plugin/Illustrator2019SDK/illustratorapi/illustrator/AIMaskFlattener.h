#ifndef __AIMaskFlattener__
#define __AIMaskFlattener__

/*
 *        Name:	AIMaskFlattener.h
 *   $Revision: 1 $
 *      Author:
 *        Date:
 *     Purpose:	Adobe Illustrator Mask Flattener Suite.
 *
 * ADOBE SYSTEMS INCORPORATED
 * Copyright 1986-2007 Adobe Systems Incorporated.
 * All rights reserved.
 *
 * NOTICE:  Adobe permits you to use, modify, and distribute this file 
 * in accordance with the terms of the Adobe license agreement 
 * accompanying it. If you have received this file from a source other 
 * than Adobe, then your use, modification, or distribution of it 
 * requires the prior written permission of Adobe.
 *
 */


/*******************************************************************************
 **
 **	Imports
 **
 **/

#ifndef __AITypes__
#include "AITypes.h"
#endif

#ifndef __AIArt__
#include "AIArt.h"
#endif

#ifndef __AIArtSet__
#include "AIArtSet.h"
#endif

#ifndef __AIPathStyle__
#include "AIPathStyle.h"
#endif


#include "AIHeaderBegin.h"

/** @file AIMaskFlattener.h */

/*******************************************************************************
 **
 **	Suite name and version
 **
 **/

#define kAIMaskFlattenerSuite					"AI Mask Flattener Suite"
#define kAIMaskFlattenerSuiteVersion5			AIAPI_VERSION(5)
#define kAIMaskFlattenerSuiteVersion			kAIMaskFlattenerSuiteVersion5
#define kAIMaskFlattenerVersion					kAIMaskFlattenerSuiteVersion

/*******************************************************************************
 **
 **	Constants
 **
 **/

/** Default resolution of raster images generated by art flattening, in DPI. */
#define kAIFlattenDefaultResolution (300.0f)
/** Minimum resolution of raster images generated by art flattening, in DPI. */
#define kAIFlattenMinResolution (1.0f)
/** Maximum resolution of raster images generated by art flattening, in DPI. */
#define kAIFlattenMaxResolution (9600.0f)
/** Minimum art-flattening balance */
#define kAIFlattenMinBalance 0
/** Maximum art-flattening balance */
#define kAIFlattenMaxBalance 100

/** Maximum number of distinct spot colors that can appear in artwork to be flattened. */
#define kAIFlattenMaxSpots			27
/** @ingroup Errors
	Art to be flattened has linked mask. See \c #AIMaskSuite and \c #AIMaskFlattenerSuite. */
#define kAIFlattenHasLinkErr		'FLnk'
/** @ingroup Errors
	Art to be flattened has too many spot colors. See \c #AIMaskFlattenerSuite. */
#define kAIFlattenTooManySpotsErr	'FSpt'


/*******************************************************************************
 **
 **	Types
 **
 **/

/** Bit flags for \c #AIFlatteningOptions::flags. See \c #AIMaskFlattenerSuite::FlattenArt().*/
typedef enum
{
	kAIFlattenDefault 						= 0,
	/** Output text outlines instead of native text */
	kAIFlattenOutlineText					= 1 << 1,
	/** Convert stroke to fill even when object is not involved in transparency */
	kAIFlattenOutlineStrokes				= 1 << 2,
	/** For regions that contain high numbers of groups and objects,
		rasterize and clip to the art bounds (unless
		\c #AIFlatteningOptions:balance is 100). This speeds
		computation and simplifies the flattened output. */
	kAIFlattenClipComplexRegions			= 1 << 3,
	/** Preserve overprint (rather than simulating it) in opaque regions */
	kAIFlattenPreserveOverprints			= 1 << 4,
	/** Show a progress bar */
	kAIFlattenShowProgress   				= 1 << 7,
	/** Preserve simple transparency (flatten blend mode) */
	kAIFlattenPreserveSimpleTransparency	= 1 << 9,
	/** Always use the planar map, even on opaque regions */
	kAIFlattenPlanarizeOpaqueRegions		= 1 << 10,
	/** Do not call \c #AIUndoSuite::UndoChanges() on error or cancel.
		(For Live Effects, for example, that handle their own undo.) */
	kAIFlattenDontUndoChangesOnError		= 1 << 11,
	/** Do not create foreign objects (art of type \c #kForeignArt) to preserve
		things like spot colors. May increase lossiness.
		See \c #kAIFOConversionFull and \c #AIFOConversionSuite. */
	kAIFlattenFOConversionFull				= 1 << 12,
	/** Convert paths in the flattened output into non-self-intersecting
		paths. See \c #kAIFOConversionSimplifyPaths and \c #AIFOConversionSuite */
	kAIFlattenFOConversionSimplifyPaths		= 1 << 13,
	/** Flatten art into a knockout group. */
	kAIFlattenIntoKnockoutGroup				= 1 << 14,
	/** Disallow colorized grayscale images in the flattened output. */
	kAIFlattenDisallowColorizedGray			= 1 << 15,
	/** Disallow NChannel/Separation/Indexed colorspace images in the flattened output. */
	kAIFlattenDisallowNChannelIndexedImages	= 1 << 16,
	/** Internal Use Only. */
	kAIUsePathDPIToLimitPrecision			= 1 << 17,
	/** Use anti-aliasing when rasterizing during flattening */
	kAIFlattenUseAntiAliasing				= 1 << 18,
} AIFlatteningFlags;


/** Obsolete. Quality factors are now controlled by
	individual \c #AIFlatteningOptions values. */
typedef enum
{
	kAIFlatteningQualityLevel0, // lower quality - more rasters
	kAIFlatteningQualityLevel1,
	kAIFlatteningQualityLevel2,
	kAIFlatteningQualityLevel3,
	kAIFlatteningQualityLevel4, // higher quality - more vectors
	kAIFlatteningQualityLevelCount

} AIFlatteningQuality;


/** Options that control how flattening occurs.
	See \c #AIMaskFlattenerSuite::FlattenArt(). */
typedef struct
{
	/** A logical OR of \c #AIFlatteningFlags. */
	ai::uint32 flags;
	/** The balance between rasters and vectors in the output
		art. An integer in the range [0..100] where 0 uses the
		most rasters and 100 uses the most vectors */
	ai::uint16 balance;
	/** Rasterization resolution for high frequency areas, in
		dots per inch (DPI).*/
	AIReal rasterResolution;
	/** Rasterization resolution for low frequency areas (smooth shades),
		in dots per inch (DPI).*/
	AIReal meshResolution;

	/** When a progress bar is specified by \c flags, the range of
		the progress values. */
	struct
	{
		/** Start of progress range, usually 0.0 */
		AIReal start;
		/** End of progress range, usually 1.0 */
		AIReal end;
	} progress;

} AIFlatteningOptions;


/** The type of information collected with
	\c #AIMaskFlattenerSuite::CollectDocumentFlatteningInfo()
	and sent to the \c #AIFlatteningInfoReceiver.

	A linked EPS is also a placed object, so the callback receives
	both types of snippets for each linked EPS, in no guaranteed order.
*/
typedef enum
{
	/** Information on linked EPS objects in flattening output. */
	kAIFlatteningInfoLinkedEPSObject,
	/** Information on placed art objects in flattening output. */
	kAIFlatteningInfoPlacedArtObject
} AIFlatteningInfoType;

/** Information on a linked EPS object created as a part of
	flattened artwork.
	*/
typedef struct
{
	/** The linked EPS object */
	AIArtHandle object;
	/** True if the linked object interacts with transparency.
		\li If it does, it must be parsed into objects for printing.
		\li If it does not, it can be passed directly to a PostScript device for printing. */
	AIBoolean interacts;
	/** True if the contents of the linked EPS can be parsed for printing.
		For example, in version 11 or later, a linked DCS cannot be parsed
		to recover the original content.*/
	AIBoolean parseOK;
} AIFlatteningInfoLinkedEPSObject;

/** Information on a placed art object created as a part of
	flattened artwork.
	 */
typedef struct
{
	/** The placed art object */
	AIArtHandle object;
} AIFlatteningInfoPlacedArtObject;

/** An information packet sent to an \c #AIFlatteningInfoReceiver. */
typedef struct
{
	/** The type of information being sent (linked EPS or placed art object). */
	AIFlatteningInfoType what;
	/** The information. The valid member is determined by
		the type. */
	union
	{
		/** Information on a linked EPS object created as a part of
			flattened artwork. */
		AIFlatteningInfoLinkedEPSObject linkedEPSObject;
		/** Information on a placed art object created as a part of
			flattened artwork.  */
		AIFlatteningInfoPlacedArtObject placedArtObject;
	} x;
} AIFlatteningInfoSnippet;

/** Prototype for a receiver of flattening information from
	\c #AIMaskFlattenerSuite::CollectDocumentFlatteningInfo().
		@param receiverdata Developer defined data passed from
			\c #AIMaskFlattenerSuite::CollectDocumentFlatteningInfo()
		@param snippet The current snippet of flattening information.
	*/
typedef AIErr (*AIFlatteningInfoReceiver) (void* receiverdata, AIFlatteningInfoSnippet* snippet);


/*******************************************************************************
 **
 **	Suite
 **
 **/

/**	@ingroup Suites
	This suite provides functions that allow you to flatten	transparent artwork into
	an equivalent collection of opaque artwork.

	\li Acquire this suite using \c #SPBasicSuite::AcquireSuite() with the constants
		\c #kAIMaskFlattenerSuite and \c #kAIMaskFlattenerVersion.
 */
typedef struct {
	/**	Flattens transparent artwork into an equivalent collection of opaque artwork.
			@param artSet The artwork to flatten.
			@param options The flags and values that control how the flattening operation is
				performed.
			@param paintOrder The paint order position of the output art, relative to the \c prep object,
				an \c #AITypes::AIPaintOrder value.
			@param prep The prepositional art object.
		*/
	AIAPI AIErr (*FlattenArt)(AIArtSet artSet, AIFlatteningOptions *options, ai::int16 paintOrder, AIArtHandle prep);

	/**	Creates a new, temporary layer at the top of the current document
		to serve  as a destination for flattened art.  When it is no longer ,
		needed, use \c #RemoveTemporaryFlatteningLayer() to delete it.
			@param layer [out] A buffer in which to return the layer object.
		*/
	AIAPI AIErr (*CreateTemporaryFlatteningLayer)(AILayerHandle *layer);

	/** Flattens all transparent artwork in the current document
		into an equivalent collection of opaque artwork in a layer, using
		flattening options obtained from the document's dictionary as set
		from the Document Setup dialog.
			@param layer The layer. Can be a temporary layer created with
				\c #CreateTemporaryFlatteningLayer().
		*/
	AIAPI AIErr (*FlattenDocumentToLayer)(AILayerHandle layer);

	/** Removes the temporary layer created with \c #CreateTemporaryFlatteningLayer()
		from the current document, along with any flattened art on the layer.
 		*/
	AIAPI AIErr (*RemoveTemporaryFlatteningLayer)();

	/**	Reports whether a layer is a temporary layer intended as a destination
		for flattened artwork.
			@param layer The layer.
			@param flattening [out] A buffer in which to return true if the layer is
				a temporary flattening layer.
		*/
	AIAPI AIErr (*IsTemporaryFlatteningLayer)(AILayerHandle layer, AIBoolean *flattening);

	/**	Displays a common flattening error message based on the
		error received back from a flattening operation.  Provides
		a uniform way to inform the user of the results of flattening.
			@param error An error code returned from a flattening operation.
		*/
	AIAPI AIErr (*ReportError)(AIErr error);

	/**	Sends information about objects created when the document was
		flattened to a developer-supplied callback. The information is sent as
		a sequence of \c #AIFlatteningInfoSnippet values,  Each snippet contains
		information about one object.

		Currently, information is supplied about linked EPS and placed art
		in the flattened output art. A linked EPS is also a placed object,
		so the callback receives both types of snippets for each linked EPS,
		in no guaranteed order.
			@param receiver The developer-supplied callback function.
			@param receiverdata Developer-defined data to pass through to the callback.
		*/
	AIAPI AIErr (*CollectDocumentFlatteningInfo)(AIFlatteningInfoReceiver receiver, void* receiverdata);

	/**	Flattens all transparent artwork in the current document
		into an equivalent collection of opaque artwork in a layer, using
		flattening options obtained from the document's dictionary as set
		from the AI or EPS Save Options dialog.
			@param layer The layer. Can be a temporary layer created with
				\c #CreateTemporaryFlatteningLayer().
			@param isEPS True if the document is to be saved to EPS, false if it
				is to be saved to AI format.
	 */
	AIAPI AIErr (*FlattenAIDocumentToLayer)(AILayerHandle layer, ASBoolean isEPS);

} AIMaskFlattenerSuite;


#include "AIHeaderEnd.h"


#endif
