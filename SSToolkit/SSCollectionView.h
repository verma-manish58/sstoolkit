//
//  SSCollectionView.h
//  SSToolkit
//
//  Created by Sam Soffes on 6/11/10.
//  Copyright 2010-2011 Sam Soffes. All rights reserved.
//

#import "SSCollectionViewItem.h"

/**
 The position in the collection view (top, middle, bottom) to which a given item is scrolled.
 */
typedef enum {
	/**
	 The collection view scrolls the items of interest to be fully visible with a minimum of movement. If the item is
	 already fully visible, no scrolling occurs. For example, if the item is above the visible area, the behavior is
	 identical to that specified by `SSCollectionViewScrollPositionTop`. This is the default.
	 */
	SSCollectionViewScrollPositionNone = UITableViewScrollPositionNone,
	
	/** The collection view scrolls the item of interest to the top of the visible collection view. */
	SSCollectionViewScrollPositionTop = UITableViewScrollPositionTop,
	
	/** The collection view scrolls the item of interest to the middle of the visible collection view. */
	SSCollectionViewScrollPositionMiddle = UITableViewScrollPositionMiddle,
	
	/** The collection view scrolls the item of interest to the bottom of the visible collection view. */
	SSCollectionViewScrollPositionBottom = UITableViewScrollPositionBottom
} SSCollectionViewScrollPosition;

/**
 The style of how headers and footers scroll.
 */
typedef enum {
	/** Headers and footer to behave like a `UITableView` with its style set to `UITableViewStylePlain`. */
	SSCollectionViewExtremitiesStyleFixed = UITableViewStylePlain,
	
	/** Headers and footer to behave like a `UITableView` with its style set to `UITableViewStyleGrouped`. */
	SSCollectionViewExtremitiesStyleScrolling = UITableViewStyleGrouped
} SSCollectionViewExtremitiesStyle;

@protocol SSCollectionViewDataSource;
@protocol SSCollectionViewDelegate;

/**
 Simple collection view.
 
 My goals are to be similar to UITableView and NSCollectionView when possible. Only scrolling vertically is currently
 supported.
 
 Editing will be my next focus. Then animating changes when data changes and an option to disable that.
 
 Note: NSIndexPath is uses the same way UITableView uses it. The `row` property is used to specify the item
 instead of row. This is done to make working with other classes that use NSIndexPath (like NSFetchedResultsController)
 easier.
 */
@interface SSCollectionView : UIView <UITableViewDataSource, UITableViewDelegate>

/**
 The object that acts as the data source of the receiving collection view.
 */
@property (nonatomic, assign) id<SSCollectionViewDataSource> dataSource;

/**
 The object that acts as the delegate of the receiving collection view.
 */
@property (nonatomic, assign) id<SSCollectionViewDelegate> delegate;

/**
 The style of the receiving collection view's headers and footers.
 
 Setting to `SSCollectionViewExtremitiesStyleFixed` will cause the headers and footer to behave like a `UITableView`
 with its style set to `UITableViewStylePlain`. Setting to `SSCollectionViewExtremitiesStyleScrolling` will cause the
 headers and footer to behave like a `UITableView` with its style set to `UITableViewStyleGrouped`. The default is
 `SSCollectionViewExtremitiesStyleFixed`.
 */
@property (nonatomic, assign) SSCollectionViewExtremitiesStyle extremitiesStyle;

/**
 The minimum column spacing.
 
 The default is `0.0`.
 */
@property (nonatomic, assign) CGFloat minimumColumnSpacing;

/**
 The spacing between each row in the receiver. This does not add space above the first row or below the last.
 
 The row spacing is in points. The default is `20.0`.
 */
@property (nonatomic, assign) CGFloat rowSpacing;

/**
 The background view of the collection view.
 */
@property (nonatomic, retain) UIView *backgroundView;

/**
 Returns an accessory view that is displayed above the collection.
 
 The default value is `nil`. The collection header view is different from a section header.
 */
@property (nonatomic, retain) UIView *collectionHeaderView;

/**
 Returns an accessory view that is displayed below the collection.
 
 The default value is `nil`. The collection footer view is different from a section footer.
 */
@property (nonatomic, retain) UIView *collectionFooterView;

/**
 A Boolean value that determines whether selecting items is enabled.
 
 If the value of this property is `YES`, selecting is enabled, and if it is `NO`, selecting is disabled. The default is
 `YES`.
 */
@property (nonatomic, assign) BOOL allowsSelection;

/**
 The internal scroll view of the collection view. The delegate must not be overridden.
 */
@property (nonatomic, retain, readonly) UIScrollView *scrollView;

/**
 The number of sections in the collection view.
 
 `SSCollectionView` gets the value returned by this method from its data source and caches it.
 */
@property (nonatomic, assign, readonly) NSUInteger numberOfSections;

/**
 Reloads the items and sections of the receiver.
 */
- (void)reloadData;

/**
 Returns a reusable collection view item object located by its identifier.
 
 @param identifier A string identifying the item object to be reused.
 
 @return A `SSCollectionViewItem` object with the associated identifier or `nil` if no such object exists in the
 reusable-item queue.
 */
- (SSCollectionViewItem *)dequeueReusableItemWithIdentifier:(NSString *)identifier;

/**
 Returns the collection view item at the specified index path.
 
 @param indexPath The index path locating the item in the receiver.
 
 @return An object representing an item of the collection or `nil` if the item is not visible or `indexPath` is out of
 range.
 
 @see indexPathForItem:
 */
- (SSCollectionViewItem *)itemPathForIndex:(NSIndexPath *)indexPath;

/**
 Returns an index path representing the row (index) and section of a given collection view item.
 
 @param item An item object of the collection view.
 
 @return An index path representing the row and section of the item or `nil` if the index path is invalid.
 
 @see itemPathForIndex:
 */
- (NSIndexPath *)indexPathForItem:(SSCollectionViewItem *)item;

/**
 Selects an item in the receiver identified by index path, optionally scrolling the item to a location in the receiver.
 
 Calling this method does cause the delegate to receive a `collectionView:willSelectRowAtIndexPath:` and
 `collectionView:didSelectRowAtIndexPath:` message, which differs from `UITableView`.
 
 @param indexPath An index path identifying an item in the receiver.
 
 @param animated `YES` if you want to animate the selection and any change in position, `NO` if
 the change should be immediate.
 
 @param scrollPosition A constant that identifies a relative position in the receiving collection view (top, middle,
 bottom) for the row when scrolling concludes.
 */
- (void)selectItemAtIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated scrollPosition:(SSCollectionViewScrollPosition)scrollPosition;

/**
 Deselects a given item identified by index path, with an option to animate the deselection.
 
 @param indexPath An index path identifying an item in the receiver.
 
 @param animated `YES` if you want to animate the deselection and `NO` if the change should be
 immediate.
 */
- (void)deselectItemAtIndexPath:(NSIndexPath *)indexPath animated:(BOOL)animated;

/**
 Scrolls the receiver until an item identified by index path is at a particular location on the screen.
 
 @param indexPath An index path that identifies an item in the table view by its row index and its section index.
 
 @param scrollPosition A constant that identifies a relative position in the receiving collection view (top, middle,
 bottom) for row when scrolling concludes.
 
 @param animated `YES` if you want to animate the change in position, `NO` if it should be
 immediate.
 */
- (void)scrollToItemAtIndexPath:(NSIndexPath *)indexPath atScrollPosition:(SSCollectionViewScrollPosition)scrollPosition animated:(BOOL)animated;

/**
 Reloads the specified item.
 
 @param indexPaths An index path that identifies an item in the collection view by its row index and its section index.
 */
- (void)reloadItemsAtIndexPaths:(NSArray *)indexPaths;

/**
 Returns the number of item (collection view items) in a specified section.
 
 @param section An index number that identifies a section of the collection.
 
 @return The number of items in the section.
 */
- (NSUInteger)numberOfItemsInSection:(NSUInteger)section;

/**
 Returns the drawing area for a specified section of the receiver.
 
 @param section An index number identifying a section of the collection view.
 
 @return A rectangle defining the area in which the collection view draws the section.
 */
- (CGRect)rectForSection:(NSUInteger)section;

/** Returns the drawing area for the header of the specified section.
 
 @param section An index number identifying a section of the collection view.
 
 @return A rectangle defining the area in which the collection view draws the section header.
 */
- (CGRect)rectForHeaderInSection:(NSUInteger)section;

/**
 Returns the drawing area for the footer of the specified section.
 
 @param section An index number identifying a section of the collection view.
 
 @return A rectangle defining the area in which the collection view draws the section footer.
 */
- (CGRect)rectForFooterInSection:(NSUInteger)section;

@end

/**
 The `SSCollectionViewDataSource` protocol is adopted by an object that mediates the application's data model for a
 `SSCollectionView` object. The data source provides the collection view object with the information it needs to
 construct and modify a collection view.
 
 As a representative of the data model, the data source supplies minimal information about the collection view's
 appearance. The collection view object's delegate—an object adopting the `SSCollectionViewDelegate` protocol—provides
 that information.
 */
@protocol SSCollectionViewDataSource <NSObject>

///------------------------------
/// Configuring a Collection View
///------------------------------

@required

/**
 Tells the data source to return the number of items in a given section of a collection view. (required)
 
 @param aCollectionView The collection view object requesting this information.
 
 @param section An index number identifying a section in `aCollectionView`.
 
 @return The number of items in `section`.
 
 @see numberOfSectionsInCollectionView:
 */
- (NSUInteger)collectionView:(SSCollectionView *)aCollectionView numberOfItemsInSection:(NSUInteger)section;

/**
 Asks the data source for an item to insert in a particular location of the collection view. (required)
 
 The returned `SSCollectionViewItem` object is frequently one that the application reuses for performance reasons. You
 should fetch a previously created item object that is marked for reuse by sending a
 `dequeueReusableItemWithIdentifier:` message to `aCollectionView`. The identifier for a reusable item object is
 assigned when the delegate initializes the item object by calling the `initWithStyle:reuseIdentifier:` method of
 `SSCollectionViewItem`.
 
 @param aCollectionView A collection view object requesting the item.
 
 @param indexPath An index path locating an item in `aCollectionView`.
 
 @return An object inheriting from `SSCollectionViewItem` that the collection view can use for the specified row. An
 assertion is raised if you return `nil`.
 */
- (SSCollectionViewItem *)collectionView:(SSCollectionView *)aCollectionView itemForIndexPath:(NSIndexPath *)indexPath;

@optional

/**
 Asks the data source to return the number of sections in the table view.
 
 @param aCollectionView An object representing the collection view requesting this information.
 
 @return The number of sections in `aCollectionView`. The default value is `1`.
 
 @see collectionView:numberOfItemsInSection:
 */
- (NSUInteger)numberOfSectionsInCollectionView:(SSCollectionView *)aCollectionView;

@end


/**
 The delegate of a `SSCollectionView` object must adopt the `SSCollectionViewDelegate` protocol. Optional methods of the
 protocol allow the delegate to manage selections, configure section headings and footers, and perform other actions.
 */
@protocol SSCollectionViewDelegate <NSObject, UIScrollViewDelegate>

///------------------------------------------
/// Configuring Items for the Collection View
///------------------------------------------

@required

/**
 Asks the delegate for the size to use for an item in a specified location.
 
 The method allows the `delegate` to specify items with varying sizes per section. This is a required method.
 
 @warning **Important:** Due to an underlying implementation detail, you should not return sizes with their width or
 height greater than `2009`.
 
 @param aCollectionView The collection view object requesting this information.
 
 @param An index path that locates a item in `aCollectionView`.
 
 @return A value that specifies the size (in points) that item should be.
 
 @exception NSException Thrown if a zero size is returned.
 */
- (CGSize)collectionView:(SSCollectionView *)aCollectionView itemSizeForSection:(NSUInteger)section;

@optional

/**
 Tells the delegate the collection view is about to draw an item for a particular index path.
 
 A collection view sends this message to its `delegate` just before it uses item to draw a portion of a row, thereby
 permitting the `delegate` to customize the `item` object before it is displayed. This method gives the `delegate` a
 chance to override state-based properties set earlier by the collection view, such as selection and background color.
 After the `delegate` returns, the collection view sets only the alpha and frame properties, and then only when
 animating items as they slide in or out.
 
 @param aCollectionView The collection view object informing the delegate of this impending event.
 
 @param item A collection view item object that `aCollectionView` is going to use when drawing the item.
 
 @param indexPath An index path locating the item in `aCollectionView`.
 
 @see collectionView:itemForIndexPath:
 
 @see prepareForReuse
 */ 
- (void)collectionView:(SSCollectionView *)aCollectionView willDisplayItem:(SSCollectionViewItem *)item atIndexPath:(NSIndexPath *)indexPath;

///--------------------
/// Managing Selections
///--------------------

/**
 Tells the delegate that a specified item is about to be selected.
 
 This method is not called until users touch an item and then lift their finger; the item isn't selected until then,
 although it is highlighted on touch-down.
 
 @param aCollectionView A collection view object informing the delegate about the impending selection.
 
 @param indexPath An index path locating the row in `aCollectionView`.
 
 @return An index-path object that confirms or alters the selected item. Return an `NSIndexPath` object other than
 `indexPath` if you want another item to be selected. Return `nil` if you don't want the item selected.
 
 @see collectionView:didSelectItemAtIndexPath:
 
 @see collectionView:willDeselectItemAtIndexPath:
 */
- (void)collectionView:(SSCollectionView *)aCollectionView willSelectItemAtIndexPath:(NSIndexPath *)indexPath;

/**
 Tells the delegate that the specified item is now selected.
 
 The delegate handles selections in this method.
 
 @param aCollectionView A collection view object informing the `delegate` about the new item selection.
 
 @param indexPath An index path locating the new selected item in `aCollectionView`. 
 
 @see collectionView:willSelectItemAtIndexPath:
 
 @see collectionView:didDeselectItemAtIndexPath:
 */
- (void)collectionView:(SSCollectionView *)aCollectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath;

/**
 Tells the delegate that a specified item is about to be deselected.
 
 This method is only called if there is an existing selection when the user tries to select a different item. The
 `delegate` is sent this method for the previously selected item.
 
 @param aCollectionView A collection view object informing the delegate about the impending selection.
 
 @param indexPath An index path locating the item in `aCollectionView` to be deselected.
 
 @return An index-path object that confirms or alters the deselected item. Return an `NSIndexPath` object other than
 `indexPath` if you want another item to be deselected. Return `nil` if you don't want the item deselected. 
 
 @see collectionView:didDeselectItemAtIndexPath:
 
 @see collectionView:willSelectItemAtIndexPath:
 */
- (void)collectionView:(SSCollectionView *)aCollectionView willDeselectItemAtIndexPath:(NSIndexPath *)indexPath;

/**
 Tells the delegate that the specified item is now deselected.
 
 The delegate handles item deselections in this method.
 
 @param aCollectionView A collection view object informing the `delegate` about the item deselection.

 @param indexPath An index path locating the deselected item in `aCollectionView`.
 
 @see collectionView:willDeselectItemAtIndexPath:
 
 @see collectionView:didSelectItemAtIndexPath:
 */ 
- (void)collectionView:(SSCollectionView *)aCollectionView didDeselectItemAtIndexPath:(NSIndexPath *)indexPath;


///--------------------------------------------
/// Modifying the Header and Footer of Sections
///--------------------------------------------

/**
 Asks the delegate for a view object to display in the header of the specified section of the collection view.
 
 The returned object can be a `UILabel` or `UIImageView` object, as well as a custom view. This method only works
 correctly when `collectionView:heightForHeaderInSection:` is also implemented.
 
 @param aCollectionView The collection view object asking for the view object.
 
 @param section An index number identifying a section of `aCollectionView`.

 @return A view object to be displayed in the header of section.
 
 @see collectionView:heightForHeaderInSection:
 */
- (UIView *)collectionView:(SSCollectionView *)aCollectionView viewForHeaderInSection:(NSUInteger)section;

/**
 Asks the delegate for a view object to display in the fotter of the specified section of the collection view.
 
 The returned object can be a `UILabel` or `UIImageView` object, as well as a custom view. This method only works
 correctly when `collectionView:heightForFooterInSection:` is also implemented.
 
 @param aCollectionView The collection view object asking for the view object.
 
 @param section An index number identifying a section of `aCollectionView`.
 
 @return A view object to be displayed in the footer of section.
 
 @see collectionView:heightForFooterInSection:
 */
- (UIView *)collectionView:(SSCollectionView *)aCollectionView viewForFooterInSection:(NSUInteger)section;

/**
 Asks the delegate for the height to use for the header of a particular section.
 
 This method allows the delegate to specify section header with varying heights.
 
 @param aCollectionView The collection view object requesting this information.
 
 @param section An index number identifying a section of `aCollectionView`.
 
 @return A floating-point value that specifies the height (in points) of the header for section.
 
 @see collectionView:viewForHeaderInSection:
 */
- (CGFloat)collectionView:(SSCollectionView *)aCollectionView heightForHeaderInSection:(NSUInteger)section;

/**
 Asks the delegate for the height to use for the footer of a particular section.
 
 This method allows the delegate to specify section footers with varying heights.
 
 @param aCollectionView The collection view object requesting this information.
 
 @param section An index number identifying a section of `aCollectionView`.
 
 @return A floating-point value that specifies the height (in points) of the footer for section.
 
 @see collectionView:viewForFooterInSection:
 */
- (CGFloat)collectionView:(SSCollectionView *)aCollectionView heightForFooterInSection:(NSUInteger)section;

@end
