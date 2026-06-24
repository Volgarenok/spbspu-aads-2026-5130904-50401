#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <string>

#include <vector.hpp>

namespace studilova
{
  class Category
	{
		public:
			Category() noexcept;
			Category(const std::string& name, Category* parent) noexcept;

			const std::string& getName() const noexcept;
			Category* getParent() const noexcept;

			void addChild(Category* child);
			const Vector< Category* >& getChildren() const noexcept;
			Vector< Category* >& getChildren() noexcept;

		private:
			std::string name_;
			Category* parent_;
			Vector< Category* > children_;
	};
}

#endif
